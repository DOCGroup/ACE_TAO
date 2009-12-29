// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  InputCL.cpp
 *
 * This file contains the InputCL class implementation for the command line
 * input adapter (helper class for command line interfaces).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <iostream>
#include <string>

#include "InputCL.h"
#include "Question.h"

using namespace UserInterface;

// Constructor.
InputCL::InputCL (std::basic_ostream<char, std::char_traits<char> >& ostrm,
                  std::basic_istream<char, std::char_traits<char> >& istrm)
: ostrm_ (ostrm),
  istrm_ (istrm)
{
  // Nothing to initialize.
};

// Destructor.
InputCL::~InputCL (void)
{
  // Nothing to do.
};

// Provide info to user.
void InputCL::info (UserInterface::Question info)
{
  this->ostrm_ << info.get_prompt ();
  this->ostrm_ << std::endl;
};

// Ask the user a question (continues asking until they provide a valid answer).
bool InputCL::ask (UserInterface::Question& question, size_t max_repeat)
{
  // Display question and get answer until receiving a valid response or maximum
  // number of repeats has been reached.
  std::string answer = "";
  for (size_t reps = 0; reps < max_repeat; reps++) {
    // Display question.
    this->ostrm_ << question.get_prompt ();
    this->ostrm_ << "  ";

    // Get answer.
    this->istrm_ >> answer;

    // Break out of question asking loop if user provided a valid answer.
    if (question.is_valid_answer (answer))
      break;

    // User provided an invalid answer, so display response.
    this->ostrm_ << question.get_invalid_response ();
    this->ostrm_ << std::endl;
  }

  // Set answer in question object and return flag indicating its validity.
  return question.answer (answer);
};







// Constructor.
QuestionInt::QuestionInt (std::string prompt, int min, int max, std::string invalid_response)
: Question (prompt, invalid_response),
  min_ (min),
  max_ (max)
{
  // Nothing to do.
};

// Destructor.
QuestionInt::~QuestionInt (void)
{
  // Nothing to do.
};

// Is an answer valid for this question?
bool QuestionInt::is_valid_answer (std::string answer)
{
  int i;
  std::istringstream answer_ss (answer);

  if (answer_ss >> i)
  { 
    if (i >= this->min_ && i <= this->max_)
      return true;
  }
  
  return false;
};

// Get answer.
int QuestionInt::get_answer_int ()
{
  int i = 0;
  std::istringstream answer_ss (this->answer_);
  
  if (answer_ss >> i)
    return i;
  
  return this->min_;
};





// Constructor
QuestionBool::QuestionBool (std::string prompt, bool def_val, std::string invalid_response)
: QuestionChoice (prompt, def_val, false, invalid_response)
{
  this->init_map ();
};

// Destructor.
QuestionBool::~QuestionBool (void)
{
  // Nothing to do.
};

// Get answer.
bool QuestionBool::get_answer_bool (void)
{
  return this->get_answer_val ();
};

// Initialize mapping with yes/no answers.
void QuestionBool::init_map (void)
{
  this->answer_map_.clear ();
  this->add_mapping ("Y", true);
  this->add_mapping ("Yes", true);
  this->add_mapping ("N", false);
  this->add_mapping ("No", false);
};

