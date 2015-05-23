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
  return false;
}

bool RunLRParser(const LRParser* lr_parser, const char* str) {
  return false;
}

