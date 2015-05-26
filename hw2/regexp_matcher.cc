// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"

void printRegExp(RegExp* regExp) {
    if (regExp->tokenType == RE_CHAR)
        printf("%c", regExp->primitiveValue);
    else if (regExp->tokenType == RE_ANYCHAR)
        printf(".");
    else if (regExp->tokenType == RE_STAR) {
        printRegExp(regExp->elements[0]);
        printf("*");
    }
    else {
        if (regExp->tokenType == RE_GROUP) printf("(");
        else if (regExp->tokenType == RE_SETCHAR) printf("[");
        for (int i=0; i<regExp->elements.size(); i++)
            printRegExp(regExp->elements[i]);
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
      printf("handle: %c ", handle);
      if (handle == ANYCHAR) {
          printf("Any single character\n");
          // 1. construct new regexp accepts any single character
          RegExp* anyCharRegExp = new RegExp(RE_ANYCHAR);
          // 2. and push it into current stack
          currentRegExp->elements.push_back(anyCharRegExp);
          cursor++;
      }
      else if (handle == STAR) {
          printf("Kleene-Star\n");
          // If no previous element to *-repeat, error
          if (currentRegExp->elements.size() == 0) return false;
          // 1. construct new *-repeat regexp on previous element
          RegExp* starRegExp = new RegExp(RE_STAR);
          starRegExp->elements.push_back(currentRegExp->elements.back());
          currentRegExp->elements.pop_back();
          // 2. and push it into current stack
          currentRegExp->elements.push_back(starRegExp);
          cursor++;
      }
      else if (handle == OPEN_GROUP) {
          printf("Opening new group of expressions\n");
          // 1. construct new group
          RegExp* groupRegExp = new RegExp(RE_GROUP);
          groupRegExp->container = currentRegExp;
          // 2. and push it into current stack
          currentRegExp->elements.push_back(groupRegExp);
          // 3. and change current stack to new group
          currentRegExp = groupRegExp;
          cursor++;
      }
      else if (handle == CLOSE_GROUP) {
          printf("Closing group of expressions\n");
          // Closing unopened group, error
          if (currentRegExp->tokenType != RE_GROUP) return false;
          currentRegExp = currentRegExp->container;
          cursor++;
      }
      else if (handle == OPEN_SET) {
          printf("Opening new set of characters\n");
          // 1. construct new set
          RegExp* setRegExp = new RegExp(RE_SETCHAR);
          setRegExp->container = currentRegExp;
          // 2. and push it into current stack
          currentRegExp->elements.push_back(setRegExp);
          // 3. and change current stack to new set
          currentRegExp = setRegExp;
          cursor++;
      }
      else if (handle == CLOSE_SET) {
          printf("Closing set of characters\n");
          // Closing unopened set, error
          if (currentRegExp->tokenType != RE_SETCHAR) return false;
          currentRegExp = currentRegExp->container;
          cursor++;
      }
      else if (handle == OR) {
          printf("OR expression\n");
          ;
      }
      else /* alphabets */ {
          printf("Single character\n");
          // 1. construct new regexp accepts any single character
          RegExp* singleCharRegExp = new RegExp(RE_CHAR);
          singleCharRegExp->primitiveValue = handle;
          // 2. and push it into current stack
          currentRegExp->elements.push_back(singleCharRegExp);
          cursor++;
      }
  }

  printRegExp(rootRegExp);
  printf("\n");

  return false;
}

bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str) {
  return false;
}
