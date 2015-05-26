// PL homework: hw2
// regexp_matcher.h

#include <vector>
using namespace std;

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

enum RegExpTokenType {
    RE_REGEXP,  // Sequence of any regular expressions
    RE_CHAR,  // Single character
    RE_ANYCHAR,  // Accepts any character
    RE_SEQCHAR,  // Sequence of single characters
    RE_SETCHAR,  // Set of single characters
    RE_GROUP,  // Similar to REGEXP but has delimiting braces
    RE_OR,  // OR-operation on two regular expressions
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
  vector<RegExp*> elements;  // Children elements
  RegExp* container;  // Reference to parent regexp

  RegExp(RegExpTokenType tkType) {
    tokenType = tkType;
    primitiveValue = '\0';
    elements = vector<RegExp*>(0);
    container = NULL;
  }
};

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
};

void printRegExp(RegExp* regExp); // Print parsed regular expression (to debug)

// Homework 2.2
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 2.2
bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_
