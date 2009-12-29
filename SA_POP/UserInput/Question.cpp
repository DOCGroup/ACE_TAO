// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Question.cpp
 *
 * This file contains the implementation of Question classes for use with
 * the command line input adapter (InputCL).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef USER_INTERFACE_QUESTION_CPP_
#define USER_INTERFACE_QUESTION_CPP_

#include <string>
#include <sstream>
#include <map>
#include <algorithm>

#include "Question.h"

using namespace UserInterface;



// Constructor.
template <typename T> 
QuestionChoice<T>::QuestionChoice (std::string prompt, T invalid_val, bool match_case, std::string invalid_response)
: Question (prompt, invalid_response),
  invalid_val_ (invalid_val),
  match_case_ (match_case)
{
  // Clear map from answers to values.
  this->answer_map_.clear ();
};

// Destructor.
template <typename T> 
QuestionChoice<T>::~QuestionChoice (void)
{
  // Nothing to do.
};

// Is an answer valid for this question?
template <typename T> bool
QuestionChoice<T>::is_valid_answer (std::string answer)
{
  std::string ans = answer;

  // If case insensitive, convert answer to upper case before checking answer map.
  if (!this->match_case_)
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);

  // If answer is not in answer map, it is not valid.
  if (this->answer_map_.find (ans) == this->answer_map_.end ())
    return false;

  return true;
};

// Get answer.
template <typename T> T
QuestionChoice<T>::get_answer_val (void)
{
  std::string ans = this->answer_;

  // If case insensitive, convert answer to upper case before checking answer map.
  if (!this->match_case_)
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);

  // Find answer in answer map.
  std::map<std::string, T>::iterator answer_iter = this->answer_map_.find (ans);

  // If answer is not in answer map, return invalid value.
  if (answer_iter == this->answer_map_.end ())
    return this->invalid_val_;

  return answer_iter->second;
};

// Set complete mapping from answers to values.
// (WARNING:  Map will completely replace all existing mappings.)
template <typename T> void
QuestionChoice<T>::set_map (const std::map<std::string, T> &answer_map)
{
  // Overwrite current mapping with new mapping (converting answers
  // to upper case, if not case sensitive).
  if (this->match_case_) {
    this->answer_map_ = answer_map;
  } else {
    // Clear current map.
    this->answer_map_.clear ();

    // Convert each answer to upper case and insert with value into answer map.
    std::string ans = "";
    for (std::map<std::string, T>::const_iterator ans_iter = answer_map.begin (); ans_iter != answer_map; ans_iter++) {
      ans = ans_iter->first;
      std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);
      this->answer_map_.insert (std::make_pair (ans, ans_iter->second));
    }
  }
};

// Add a mapping from an answer to a value (or replace with provided
// value if a mapping already exists for the answer).
template <typename T> void
QuestionChoice<T>::add_mapping (std::string answer, T value)
{
  std::string ans = answer;

  // If case insensitive, convert answer to upper case before inserting in answer map.
  if (!this->match_case_)
    std::transform(ans.begin(), ans.end(), ans.begin(), ::toupper);

  this->answer_map_.insert (std::make_pair (ans, value));
};




#endif /* USER_INTERFACE_QUESTION_CPP_ */
