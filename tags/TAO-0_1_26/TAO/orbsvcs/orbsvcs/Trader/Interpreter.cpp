// $Id$

#include "Interpreter.h"

TAO_Interpreter::~TAO_Interpreter (void)
{
  delete root_;
}

int
TAO_Interpreter::build_tree(const char* constraints)
{
  TAO_Lex_String_Input::reset((char*)constraints);  
  int return_value = 0;

  yyval.constraint_ = 0;
  return_value = ::yyparse();
  
  if (return_value == 0 && yyval.constraint_ != 0)
    this->root_ = yyval.constraint_;
  else
    {
      while (yylex() > 0)
	;
      this->root_ = 0;
    }
  
  return return_value;
}


int
TAO_Interpreter::is_empty_string(const char* str)
{
  int return_value = 0;

  if (str != 0)
    {
      int i = 0;
      while (str[i] != '\0')
	{
	  if (str[i] != ' ')
	    break;

	  i++;
	}

      if (str[i] == '\0')
	return_value = 1;
    }

  return return_value;
}

