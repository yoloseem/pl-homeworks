// PL homework: hw2
// lr_parser.h

#ifndef _PL_HOMEWORK_LR_PARSER_H_
#define _PL_HOMEWORK_LR_PARSER_H_

#include <map>
#include <set>

enum LRAction {
  INVALID = 0,
  SHIFT = 1,
  REDUCE = 2,
  ACCEPT = 3,
  GOTO = 4
};

// The data fields for each type of action:
//   SHIFT : symbol = the input character, next_state = the next state.
//   REDUCE : symbol = lhs of the rule, next_state = the rule id.
//   ACCEPT : none.
//   GOTO : symbol = the state on the stack top, next_state = the next state.

struct LRTableElement {
  int state;
  int symbol;
  LRAction action;
  int next_state;
};

struct LRRule {
  int id;
  int lhs_symbol;
  int num_rhs;
};

struct LRParser {
  // Design your LRParser structure.
};

// Homework 2.1
bool BuildLRParser(const LRTableElement* elements, int num_elements,
                   const LRRule* rules, int num_rules,
                   LRParser* lr_parser);

// Homework 2.1
bool RunLRParser(const LRParser* lr_parser, const char* str);

#endif //_PL_HOMEWORK_LR_PARSER_H_
