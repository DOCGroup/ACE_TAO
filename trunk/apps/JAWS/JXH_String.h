// -*- C++ -*-
// JXH_String.h

#ifndef JXH_STRING_H
#define JXH_STRING_H

#include <stdlib.h>
#include <string.h>

class String
{
public:

  String() : str_(0) {}

  String(const char *s)   : str_(::strcpy(new char[::strlen(s)+1], s)) {}
  String(const String &s) : str_(::strcpy(new char[::strlen(s)+1], s)) {}

  ~String() { if (str_) delete [] str_; }

  operator char* () const { return str_; }
  char & operator [] (int i) const { return str_[i]; }

  int operator == (const char *s) const { return ::strcmp(str_, s) == 0; }
  int operator >  (const char *s) const { return ::strcmp(str_, s) == 1; }
  int operator <  (const char *s) const { return ::strcmp(str_, s) == -1; }
  int operator >= (const char *s) const { return ::strcmp(str_, s) >= 0; }
  int operator <= (const char *s) const { return ::strcmp(str_, s) <= 0; }

  String & operator = (const char *s)
  { delete [] str_; str_ = ::strcpy(new char[strlen(s)+1], s);
    return *this; }

  String & operator += (const char *s)
  { char * ss = ::strcpy(new char[::strlen(str_)+::strlen(s)+1], str_);
    delete [] str_; str_ = ::strcat(ss, s); return *this; }

  int length (void) const { return ::strlen(str_); }

private:
  char * str_;
};

#endif
