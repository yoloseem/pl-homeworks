// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
};

// Homework 2.2
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 2.2
bool RunRegExpMatcher(const RegExpMatcher* regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

