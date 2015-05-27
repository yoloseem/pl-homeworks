// PL homework: hw2
// regexp_matcher.h

#include <map>
#include <set>
#include <vector>
using namespace std;

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

struct TableElement {
  int state;
  char input_char;
  int next_state;
  TableElement(int stateId, char transitionChar, int nextStateId) {
      state = stateId;
      input_char = transitionChar;
      next_state = nextStateId;
  }
};
struct FiniteStateAutomaton {
  int start_state;
  std::vector<int> states;
  std::vector<char> alphabets;
  std::vector<int> accept_states;
  std::map< int, std::map<char, int> > transitions;
};
bool RunFSA(const FiniteStateAutomaton* fsa, const char* str);
bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa);
bool BuildFSA(const char* regex, FiniteStateAutomaton* fsa);

enum RegExpTokenType {
    RE_REGEXP,  // Sequence of any regular expressions
    RE_CHAR,  // Single character
    RE_ANYCHAR,  // Accepts any character
    RE_SETCHAR,  // Set of single characters
    RE_GROUP,  // Similar to REGEXP but has delimiting braces
    RE_STAR,  // 0-or-more repetitions of single regular expression
};
const char ANYCHAR = '.';
const char OPEN_GROUP = '(';
const char CLOSE_GROUP = ')';
const char OPEN_SET = '[';
const char CLOSE_SET = ']';
const char STAR = '*';
const char OR = '|';

struct RegExp {
  RegExpTokenType tokenType;
  char primitiveValue;  // Used only when tokenType is CHARACTER
  vector< vector<RegExp*> > elements;  // Children elements
  RegExp* container;  // Reference to parent regexp
  int or_ops_count;

  RegExp(RegExpTokenType tkType) {
    tokenType = tkType;
    primitiveValue = '\0';
    elements = vector< vector<RegExp*> >(1);
    elements[0].resize(0);
    container = NULL;
    or_ops_count = 0;
  }
};

struct RegExpMatcher {
  RegExp* regExp;
  FiniteStateAutomaton* fsa;
};

void printRegExp(RegExp* regExp); // Print parsed regular expression (to debug)

int FindTransitions(vector<RegExp*>* regExpElements,
                    vector<TableElement>* fsaElements,
                    set<char> alphabets,
                    int startState, int finalState);
// Find FSA transitions from regExp and update it into fsaElements
// Returns maximum used id value that will be used in next finding part

// Homework 2.2
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 2.2
bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_
