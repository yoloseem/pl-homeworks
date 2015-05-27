// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <stack>
#include <vector>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const LRTableElement* elements, int num_elements,
                   const LRRule* rules, int num_rules,
                   LRParser* lr_parser) {
  int i;

  for (i=0; i<num_rules; i++)
    lr_parser->rules.push_back(rules[i]);

  for (i=0; i<num_elements; i++) {
    if (elements[i].action == GOTO)
      lr_parser->goto_elems.push_back(elements[i]);
    else
      lr_parser->action_elems.push_back(elements[i]);
  }

  return true;
}

bool RunLRParser(const LRParser* lr_parser, const char* str) {
  // parse_stack[] = (SYMBOL, STATE)[]
  stack< pair<int, int> > parse_stack;
  parse_stack.push( make_pair(0, 0) );

  LRAction next_action;
  int i, j, top_state, reduce_rule_id, input, input_cursor = 0;
  LRTableElement tbl_elem;
  LRRule reduce_rule;
  while (true) {
    input = str[input_cursor];
    if (input == '\0') break;

    top_state = parse_stack.top().second;
    for (i=0; i<lr_parser->action_elems.size(); i++) {
      if (lr_parser->action_elems[i].state != top_state) continue;
      if (lr_parser->action_elems[i].symbol != input) continue;
      if (lr_parser->action_elems[i].action == ACCEPT) return true;
      else if (lr_parser->action_elems[i].action == SHIFT) {
        parse_stack.push( make_pair(
          input,
          lr_parser->action_elems[i].next_state
        ) );
        input_cursor++;
        break;
      }
      else if (lr_parser->action_elems[i].action == REDUCE) {
        for (j=0; j<lr_parser->rules.size(); j++) {
          if (lr_parser->rules[j].id == lr_parser->action_elems[i].next_state)
            break;
        }
        if (j >= lr_parser->rules.size()) return false;
        reduce_rule = lr_parser->rules[j];
        for (j=0; j<reduce_rule.num_rhs; j++) {
          if (parse_stack.empty()) return false;
          parse_stack.pop();
        }
        top_state = parse_stack.top().second;
        for (j=0; j<lr_parser->goto_elems.size(); j++) {
          if (lr_parser->goto_elems[j].state == top_state &&
              lr_parser->goto_elems[j].symbol == reduce_rule.lhs_symbol) {
            parse_stack.push( make_pair(
              reduce_rule.lhs_symbol,
              lr_parser->goto_elems[j].next_state
            ) );
            break;
          }
        }
        if (j >= lr_parser->goto_elems.size()) return false;
        else break;
      }
    }
    if (i >= lr_parser->action_elems.size()) return false;
  }

  return false;
}
