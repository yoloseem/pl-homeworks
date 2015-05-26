// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"

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

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
  // Returns false when parse error

  RegExp *rootRegExp = new RegExp(RE_REGEXP);

  int cursor = 0;
  char handle;
  RegExp* currentRegExp = rootRegExp;
  while((handle = regexp[cursor]) != '\0') {
      if (handle == ANYCHAR) {
          // 1. construct new regexp accepts any single character
          RegExp* anyCharRegExp = new RegExp(RE_ANYCHAR);
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(anyCharRegExp);
          cursor++;
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
          cursor++;
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
          cursor++;
      }
      else if (handle == CLOSE_GROUP) {
          // Closing unopened group, error
          if (currentRegExp->tokenType != RE_GROUP) return false;
          currentRegExp = currentRegExp->container;
          cursor++;
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
          cursor++;
      }
      else if (handle == CLOSE_SET) {
          // Closing unopened set, error
          if (currentRegExp->tokenType != RE_SETCHAR) return false;
          currentRegExp = currentRegExp->container;
          cursor++;
      }
      else if (handle == OR) {
          // Break regexp if OR(|) appears
          currentRegExp->or_ops_count++;
          currentRegExp->elements.push_back(vector<RegExp*>(0));
          cursor++;
      }
      else /* alphabets */ {
          // 1. construct new regexp accepts any single character
          RegExp* singleCharRegExp = new RegExp(RE_CHAR);
          singleCharRegExp->primitiveValue = handle;
          // 2. and push it into current stack
          currentRegExp->elements[currentRegExp->or_ops_count]
            .push_back(singleCharRegExp);
          cursor++;
      }
  }

  return true;
}

bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str) {
  return false;
}
