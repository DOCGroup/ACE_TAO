/* -*- C++ -*- */

// $Id$

#include "algorithm.hpp"

inline bool is_pure_alpha_word(const std::string& word)
{
  return common::satisfies_each(word.begin(), word.end(), isalpha);
}
