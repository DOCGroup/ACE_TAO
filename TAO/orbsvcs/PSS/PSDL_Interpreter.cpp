// -*- C++ -*-
// $Id$

#include "PSDL_Interpreter.h"
#include "PSDL_Node.h"
#include "PSDL_y.h"



TAO_SYNCH_MUTEX TAO_PSDL_Interpreter::parserMutex__;

TAO_PSDL_Interpreter::TAO_PSDL_Interpreter (void)
  : root_ (0)
{
}

TAO_PSDL_Interpreter::~TAO_PSDL_Interpreter (void)
{
  delete this->root_;
}

int
TAO_PSDL_Interpreter::build_tree (const char* nodes)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    TAO_PSDL_Interpreter::parserMutex__,
                    -1);

  TAO_Lex_String_Input::reset ((char*)nodes);

  yyval = 0;
  void *result_ptr = 0;
  int return_value = TAO_PSDL_yyparse (result_ptr);

  if (return_value == 0 && yyval != 0)
    {
      this->root_ = yyval;
    }
  else
    {
      this->root_ = 0;
    }

  return return_value;
}

int
TAO_PSDL_Interpreter::is_empty_string (const char* str)
{
  int return_value = 0;

  if (str != 0)
    {
      int i = 0;

      while (str[i] != '\0')
        {
          if (str[i] != ' ')
            {
              break;
            }

          i++;
        }

      if (str[i] == '\0')
        {
          return_value = 1;
        }
    }

  return return_value;
}

char* TAO_Lex_String_Input::string_ = 0;
char* TAO_Lex_String_Input::current_ = 0;
char* TAO_Lex_String_Input::end_ = 0;

// Routine to have Lex read its input from the node string.

int
TAO_Lex_String_Input::copy_into (char* buf,
                                 int max_size)
{
  int chars_left =  TAO_Lex_String_Input::end_ - TAO_Lex_String_Input::current_;
  int n = max_size > chars_left ? chars_left : max_size;

  if (n > 0)
    {
      ACE_OS::memcpy (buf,
                      TAO_Lex_String_Input::current_,
                      n);
      TAO_Lex_String_Input::current_ += n;
    }

  return n;
}

void
TAO_Lex_String_Input::reset (char* input_string)
{
  TAO_Lex_String_Input::string_ = input_string;
  TAO_Lex_String_Input::current_ = input_string;
  TAO_Lex_String_Input::end_ =
    input_string + ACE_OS::strlen (TAO_Lex_String_Input::string_);
}
