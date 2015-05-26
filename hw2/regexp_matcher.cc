// PL homework: hw2
// regexp_matcher.cc

#include "regexp_matcher.h"

void printRegExp(RegExp* regExp) {
    if (regExp->tokenType == RE_CHAR)
        printf("%c", regExp->primitiveValue);
    else if (regExp->tokenType == RE_ANYCHAR)
        printf(".");
    else if (regExp->tokenType == RE_STAR) {
        printRegExp(&regExp->elements[0]);
        printf("*");
    }
    else {
        int i;
        for (i=0; i<regExp->elements.size(); i++) {
            printRegExp(&regExp->elements[i]);
        }
    }
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
  return false;
}

bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str) {
  return false;
}
