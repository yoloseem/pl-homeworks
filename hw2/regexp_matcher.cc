// PL homework: hw2
// regexp_matcher.cc

#include <iostream>
#include <stdexcept>
#include <iterator>
#include <cstring>
#include <algorithm>
#include <set>
#include <vector>

#include "regexp_matcher.h"

#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

const char kEps = '#';

bool BuildNFA(const TableElement* elements, int num_elements,
              const int* accept_states_array, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  LOG << "num_elements: " << num_elements << endl;
  if (num_elements <= 0) return false;

  int i, j;

  set<int> sStates, sAlphabets, sAcceptStates;
  for (i=0; i<num_elements; i++) {
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
    if (elements[i].input_char != kEps)
        sAlphabets.insert(elements[i].input_char);
  }
  for (i=0; i<num_accept_states; i++)
    sAcceptStates.insert(accept_states_array[i]);
  vector<char> alphabets (sAlphabets.begin(), sAlphabets.end());

  vector<int> statesLoop;
  vector< set<int> > states, statesToConstruct;
  set<int> startState;
  startState.insert(elements[0].state);
  // Construct start state with epsiolon-moves
  while (true) {
    int previous_cnt = startState.size();
    statesLoop = vector<int>(startState.begin(), startState.end());
    for (i=0; i<statesLoop.size(); i++) {
      for (j=0; j<num_elements; j++) {
        if (elements[j].state == statesLoop[i] &&
            elements[j].input_char == kEps)
          startState.insert(elements[j].next_state);
      }
    }
    if (startState.size() == previous_cnt) {
      // Discovery completed
      break;
    }
  }
  statesToConstruct.push_back(startState);

  set<int> handling_state, current_states, next_states, visited_states;
  vector< pair<pair< set<int>, set<int> >, char> > trans_edges;
  while (statesToConstruct.size()) {
    handling_state = statesToConstruct.back();
    statesToConstruct.pop_back();
    states.push_back(handling_state);

    // Discovery next states with epsilon-moves
    for (i=0; i<alphabets.size(); i++) {
      current_states.clear();
      current_states.insert(handling_state.begin(), handling_state.end());
      next_states.clear();
      visited_states.clear();

      while (current_states.size()) {
        statesLoop = vector<int>(current_states.begin(), current_states.end());
        current_states.clear();
        for (vector<int>::iterator it=statesLoop.begin();
             it!=statesLoop.end(); ++it) {
          visited_states.insert(*it);
          for(j=0; j<num_elements; j++) {
            if (elements[j].state == *it) {
              if (elements[j].input_char == alphabets[i]) {
                next_states.insert(elements[j].next_state);
              }
              else if (elements[j].input_char == kEps) {
                bool visited = (visited_states.find(elements[j].next_state)
                                != visited_states.end());
                if (!visited)
                  current_states.insert(elements[j].next_state);
              }
            }
          }
        }
      }

      if (next_states.size() == 0) { continue; }
      pair<pair< set<int>, set<int> >, char> transition_edge;
      transition_edge = make_pair(make_pair(handling_state, next_states),
                                  alphabets[i]);
      trans_edges.push_back(transition_edge);

      for (j=0; j<states.size(); j++)
        if (states[j] == next_states) break;
      if (j == states.size()) {
        for (j=0; j<statesToConstruct.size(); j++)
          if (statesToConstruct[j] == next_states) break;
        if (j == statesToConstruct.size()) {
          statesToConstruct.push_back(next_states);
        }
      }
    }
  }

  // Let's build DFA
  fsa->alphabets = alphabets;
  fsa->start_state = 0;

  bool isAcceptedState;
  for (i=0; i<states.size(); i++) {
    isAcceptedState = false;
    fsa->states.push_back(i);
    LOG << "q" << i << " => {";
    for (set<int>::iterator it=states[i].begin(); it!=states[i].end(); ++it) {
      if (it != states[i].begin()) { LOG << ", "; }
      LOG << "q" << *it;
      for (j=0; j<num_accept_states; j++) {
        if (*it == accept_states_array[j]) {
          isAcceptedState = true;
          LOG << "(accept)";
          break;
        }
      }
      if (isAcceptedState) fsa->accept_states.push_back(i);
    }
    LOG << "}" << endl;
  }

  LOG << "|Q| = " << fsa->states.size() << ". Q = {";
  for (int i=0; i < fsa->states.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << "q" << fsa->states[i];
  }
  LOG << "}" << endl;
  LOG << "|Σ| = " << fsa->alphabets.size() << ". Σ = {";
  for (int i=0; i < fsa->alphabets.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << fsa->alphabets[i];
  }
  LOG << "}" << endl;
  LOG << "|F| = " << fsa->accept_states.size() << ". F = {";
  for (int i=0; i < fsa->accept_states.size(); i++) {
    if (i > 0) { LOG << ", "; }
    LOG << "q" << fsa->accept_states[i];
  }
  LOG << "}" << endl;

  LOG << "q_start = q" << fsa->start_state << endl;

  for (i=0; i<trans_edges.size(); i++) {
    set<int> curstate, nextstate;
    curstate = trans_edges[i].first.first;
    nextstate = trans_edges[i].first.second;
    char input_char = trans_edges[i].second;

    int curstate_i = -1, nextstate_i = -1;
    for (j=0; j<states.size(); j++) {
      if (states[j] == curstate) {
        curstate_i = j;
      }
      if (states[j] == nextstate) {
        nextstate_i = j;
      }
      if (curstate_i > -1 && nextstate_i > -1) break;
    }
    if (curstate_i == -1 || nextstate_i == -1) {
      cerr << "Converting failed.." << endl;
      return false;
    }
    fsa->transitions[curstate_i][input_char] = nextstate_i;

    LOG << "δ(q" << curstate_i << ", " << input_char << ") = ";
    LOG << "q" << nextstate_i << endl;
  }

  return true;
}

bool RunFSA(const FiniteStateAutomaton* fsa, const char* str) {
  const int n = strlen(str);
  LOG << "Query: '" << str << "' (Length: " << n << ")" << endl;

  int current_state = fsa->start_state;
  while (str[0] != '\0') {
    char input_char = str[0];
    int next_state;

    try {
      next_state = fsa->transitions.at(current_state).at(input_char);
    } catch (const out_of_range& oor) {
      LOG << "No transition, so FSA does not accept given input" << endl;
      return false;
    }
    LOG << "δ(q" << current_state << ", " << input_char << ") = q";
    LOG << next_state << endl;

    str++;
    current_state = next_state;
  }

  bool currentIsFinal = (find(fsa->accept_states.begin(),
                              fsa->accept_states.end(),
                              current_state) != fsa->accept_states.end());
  return currentIsFinal;
}

bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accepts,
              FiniteStateAutomaton* fsa) {
  return BuildNFA(elements, num_elements, accept_states, num_accepts, fsa);
}

void printRegExp(RegExp* regExp) {
    if (regExp->tokenType == RE_CHAR)
        printf("%c", regExp->primitiveValue);
    else if (regExp->tokenType == RE_ANYCHAR)
        printf(".");
    else if (regExp->tokenType == RE_STAR) {
        printRegExp(regExp->elements[0][0]);
        printf("*");
    }
    else {
        if (regExp->tokenType == RE_GROUP) printf("(");
        else if (regExp->tokenType == RE_SETCHAR) printf("[");
        for (int i=0; i<=regExp->or_ops_count; i++) {
          if (i > 0) printf("|");
          for (int j=0; j<regExp->elements[i].size(); j++)
              printRegExp(regExp->elements[i][j]);
        }
        if (regExp->tokenType == RE_GROUP) printf(")");
        else if (regExp->tokenType == RE_SETCHAR) printf("]");
    }
}

int FindTransitions(RegExp* regExp,
                    vector<TableElement>* fsaElements,
                    set<char> *alphabetsSet,
                    int startState, int finalState) {
    vector<char> alphabets = vector<char>(alphabetsSet->begin(),
                                          alphabetsSet->end());

    printf("FindTransitions(");
    printRegExp(regExp);
    printf(")\n");
    printf("Configured startState = %d, finalState = %d\n",
           startState, finalState);

    int curId = finalState;
    if (regExp->tokenType == RE_REGEXP || regExp->tokenType == RE_GROUP) {
        for (int i=0; i<=regExp->or_ops_count; i++) {
            curId++;
            TableElement* startElem =
                new TableElement(startState, kEps, curId);
            fsaElements->push_back(*startElem);
            for (int j=0; j<regExp->elements[i].size(); j++) {
                if (regExp->elements[i][j]->tokenType == RE_CHAR) {
                    TableElement* elem = new TableElement(
                        curId,
                        regExp->elements[i][j]->primitiveValue,
                        curId + 1
                    );
                    fsaElements->push_back(*elem);
                    curId++;
                }
                else if (regExp->elements[i][j]->tokenType == RE_ANYCHAR) {
                    for(int k=0; k<alphabets.size(); k++) {
                        TableElement* elem =
                            new TableElement(curId, alphabets[k], curId + 1);
                        fsaElements->push_back(*elem);
                        curId++;
                    }
                }
            }
            TableElement* endElem = new TableElement(curId, kEps, finalState);
            fsaElements->push_back(*endElem);
        }
    }
    return curId;
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
  // Returns false when parse error

  RegExp *rootRegExp = new RegExp(RE_REGEXP);
  set<char> alphabets;
  char handle;
  int cursor = 0;

  while((handle = regexp[cursor++]) != '\0') {
      if (handle != ANYCHAR && handle != STAR && handle != OR &&
          handle != OPEN_GROUP && handle != CLOSE_GROUP &&
          handle != OPEN_SET && handle != CLOSE_SET)
        alphabets.insert(handle);
  }

  cursor = 0;
  RegExp* currentRegExp = rootRegExp;
  while((handle = regexp[cursor++]) != '\0') {
      if (handle == ANYCHAR) {
          // 1. construct new regexp accepts any single character
          RegExp* anyCharRegExp = new RegExp(RE_ANYCHAR);
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(anyCharRegExp);
      }
      else if (handle == STAR) {
          // If no previous element to *-repeat, error
          if (currentRegExp->elements[0].size() == 0) return false;
          // 1. construct new *-repeat regexp on previous element
          RegExp* starRegExp = new RegExp(RE_STAR);
          starRegExp->elements[0].push_back(
            currentRegExp->elements[currentRegExp->or_ops_count].back());
          currentRegExp->elements[currentRegExp->or_ops_count].pop_back();
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(starRegExp);
      }
      else if (handle == OPEN_GROUP) {
          // 1. construct new group
          RegExp* groupRegExp = new RegExp(RE_GROUP);
          groupRegExp->container = currentRegExp;
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(groupRegExp);
          // 3. and change current stack to new group
          currentRegExp = groupRegExp;
      }
      else if (handle == CLOSE_GROUP) {
          // Closing unopened group, error
          if (currentRegExp->tokenType != RE_GROUP) return false;
          currentRegExp = currentRegExp->container;
      }
      else if (handle == OPEN_SET) {
          // 1. construct new set
          RegExp* setRegExp = new RegExp(RE_SETCHAR);
          setRegExp->container = currentRegExp;
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(setRegExp);
          // 3. and change current stack to new set
          currentRegExp = setRegExp;
      }
      else if (handle == CLOSE_SET) {
          // Closing unopened set, error
          if (currentRegExp->tokenType != RE_SETCHAR) return false;
          currentRegExp = currentRegExp->container;
      }
      else if (handle == OR) {
          // Break regexp if OR(|) appears
          currentRegExp->or_ops_count++;
          currentRegExp->elements.push_back(vector<RegExp*>(0));
      }
      else /* alphabets */ {
          // 1. construct new regexp accepts any single character
          RegExp* singleCharRegExp = new RegExp(RE_CHAR);
          singleCharRegExp->primitiveValue = handle;
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(singleCharRegExp);
      }
  }

  vector<TableElement> fsa_elements;
  fsa_elements.clear();

  FindTransitions(rootRegExp, &fsa_elements, &alphabets, 0, 1);
  printf("fsa_elements.size() = %lu\n", fsa_elements.size());
  for(int i=0; i<fsa_elements.size(); i++) {
      printf("%d =(%c)=> %d\n",
             fsa_elements[i].state,
             fsa_elements[i].input_char,
             fsa_elements[i].next_state);

  }
  set<int> accept_states_set;
  /* TODO: Discovery all final states */
  accept_states_set.insert(1);
  int statesCount;
  do {
    statesCount = accept_states_set.size();
    for(int i=0; i<fsa_elements.size(); i++) {
        if (fsa_elements[i].input_char != kEps) continue;
        vector<int> accept_states_vec =
            vector<int>(accept_states_set.begin(), accept_states_set.end());
        for(int j=0; j<accept_states_vec.size(); j++) {
            if (fsa_elements[i].next_state == accept_states_vec[j])
                accept_states_set.insert(fsa_elements[i].state);
        }
    }
  } while(statesCount != accept_states_set.size());
  vector<int> accept_states =
      vector<int>(accept_states_set.begin(), accept_states_set.end());

  regexp_matcher->fsa = new FiniteStateAutomaton();
  return BuildFSA(&fsa_elements[0], fsa_elements.size(),
                  &accept_states[0], accept_states.size(),
                  regexp_matcher->fsa);
}

bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str) {
  return RunFSA(regexp_matcher->fsa, str);
}
