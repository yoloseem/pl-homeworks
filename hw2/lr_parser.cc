// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
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
  return false;
}
