/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Constraint.cpp
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================


#include "Constraint.h"

char* TAO_Lex_String_Input::string_ = 0;
char* TAO_Lex_String_Input::current_ = 0;
char* TAO_Lex_String_Input::end_ = 0;

// Routine to have Lex read its input from the constraint string.

int
TAO_Lex_String_Input::copy_into(char* buf, int max_size)
{
  int chars_left =  TAO_Lex_String_Input::end_ - TAO_Lex_String_Input::current_;
  int n = (max_size > chars_left) ? chars_left : max_size;

  
  if (n > 0)
    {
      memcpy(buf, TAO_Lex_String_Input::current_, n);
      TAO_Lex_String_Input::current_ += n;
    }
  
  return n;
}

void
TAO_Lex_String_Input::reset(char* input_string)
{    
  TAO_Lex_String_Input::string_ = input_string;
  TAO_Lex_String_Input::current_ = input_string;
  TAO_Lex_String_Input::end_ = input_string +
    ACE_OS::strlen(TAO_Lex_String_Input::string_);
}
