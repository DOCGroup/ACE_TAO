/* -*- C++ -*- */

// $Id$

#include <cstdlib>

bool common::ltstr::operator()(const char* s1, const char* s2) const
{
  return strcmp(s1, s2) < 0;
} 

bool common::ltstr::operator()(const std::string& s1,
                               const std::string& s2) const
{
  return strcmp(s1.c_str(), s2.c_str()) < 0;
}

