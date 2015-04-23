// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"

#define DISABLE_LOG false // true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

const char kEps = '#';

using namespace std;

bool CheckIfNFA(const TableElement* elements, int num_elements) {
  const bool NFA = true, DFA = false;

  set< pair<int, char> > sTransDomains;
  set<int> sStates;
  set<char> sAlphabets;

  for (int i=0; i<num_elements; i++) {
    if (elements[i].input_char == kEps) {
      LOG << "Epsilon-move found on state " << elements[i].state << endl;
      return NFA;
    }

    pair<int, char> pMoving = make_pair(elements[i].state,
                                        elements[i].input_char);
    bool multipleTrans = (sTransDomains.find(pMoving) != sTransDomains.end());
    if (multipleTrans) {
      LOG << "Multiple transitions found on state " << elements[i].state
          << " with input " << elements[i].input_char << endl;
      return NFA;
    }
    sTransDomains.insert(pMoving);

    if (elements[i].input_char != kEps) {
      // Of course epsilon-move cannot enter here, but to be defensive..
      sAlphabets.insert(elements[i].input_char);
    }
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
  }
  vector<char> alphabets (sAlphabets.begin(), sAlphabets.end());
  vector<int> states (sStates.begin(), sStates.end());

  LOG << "Detected alphabets:";
  for (int i=0; i<alphabets.size(); i++) LOG << ' ' << alphabets[i];
  LOG << endl << "Detected states:";
  for (int i=0; i<states.size(); i++) LOG << ' ' << states[i];
  LOG << endl;

  for (int i=0; i<states.size(); i++) {
    for (int j=0; j<alphabets.size(); j++) {
      pair<int, char> pMoving = make_pair(states[i], alphabets[j]);
      bool missingTrans = (sTransDomains.find(pMoving) == sTransDomains.end());
      if (missingTrans) {
        LOG << "Missing transition detected: δ(" << states[i] << ", "
            << alphabets[j] << ")" << endl;
        return NFA;
      }
    }
  }

  return DFA;
}

bool BuildDFA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  LOG << "num_elements: " << num_elements << endl;
  if (num_elements <= 0) return false;

  set<int> sStates, sAlphabets, sAcceptStates;
  for (int i=0; i<num_elements; i++) {
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
    if (elements[i].input_char != kEps)
        sAlphabets.insert(elements[i].input_char);
  }
  for (int i=0; i<num_accept_states; i++)
    sAcceptStates.insert(accept_states[i]);

  fsa->states = vector<int>(sStates.begin(), sStates.end());
  fsa->alphabets = vector<char>(sAlphabets.begin(), sAlphabets.end());
  fsa->accept_states = vector<int>(sAcceptStates.begin(), sAcceptStates.end());

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

  fsa->start_state = elements[0].state;
  LOG << "q_start = q" << fsa->start_state << endl;
  for (int i=0; i<num_elements; i++) {
    fsa->transitions[elements[i].state]
                    [elements[i].input_char] = elements[i].next_state;
  }

  for (int i=0; i < fsa->states.size(); i++) {
    for (int j=0; j < fsa->alphabets.size(); j++) {
      LOG << "δ(q" << fsa->states[i] << ", " << fsa->alphabets[j] << ") = ";
      LOG << "q" << fsa->transitions[fsa->states[i]][fsa->alphabets[j]];
      LOG << endl;
    }
  }

  return true;
}

bool BuildNFA(const TableElement* elements, int num_elements,
              const int* accept_states_array, int num_accept_states,
              FiniteStateAutomaton* fsa) {
  LOG << "num_elements: " << num_elements << endl;
  if (num_elements <= 0) return false;

  set<int> sStates, sAlphabets, sAcceptStates;
  for (int i=0; i<num_elements; i++) {
    sStates.insert(elements[i].state);
    sStates.insert(elements[i].next_state);
    if (elements[i].input_char != kEps)
        sAlphabets.insert(elements[i].input_char);
  }
  for (int i=0; i<num_accept_states; i++)
    sAcceptStates.insert(accept_states_array[i]);
  vector<char> alphabets (sAlphabets.begin(), sAlphabets.end());

  vector< set<int> > states, statesToConstruct;
  set<int> startState;
  startState.insert(elements[0].state);
  statesToConstruct.push_back(startState);

  int i, j;
  set<int> handling_state, current_states, next_states, visited_states;
  vector<int> statesLoop;
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

// Homework 1.1
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
      cerr << "Out Of Range exception raised" << endl;
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

// Homework 1.1 and 1.2
bool BuildFSA(const TableElement* elements, int num_elements,
              const int* accept_states, int num_accepts,
              FiniteStateAutomaton* fsa) {
  bool isNFA = CheckIfNFA(elements, num_elements);
  LOG << "Given FSA is " << (isNFA ? "NFA" : "DFA") << endl;
  if (isNFA) {
    return BuildNFA(elements, num_elements, accept_states, num_accepts, fsa);
  } else {
    return BuildDFA(elements, num_elements, accept_states, num_accepts, fsa);
  }
}

// Homework 1.3
bool BuildFSA(const char* regex, FiniteStateAutomaton* fsa) {
  // Implement this function.
  return false;
}

