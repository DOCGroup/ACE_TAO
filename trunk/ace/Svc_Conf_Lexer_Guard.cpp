// -*- C++ -*-

#include "ace/Svc_Conf.h"
#include "ace/Object_Manager.h"

#include "ace/Svc_Conf_Lexer_Guard.h"

ACE_RCSID (ace,
           Svc_Conf_Lexer_Guard,
           "$Id$")

ACE_Svc_Conf_Lexer_Guard::ACE_Svc_Conf_Lexer_Guard (FILE *file)
{
  // External synchronization is required.

  ::ace_yy_push_buffer (file);
}

ACE_Svc_Conf_Lexer_Guard::ACE_Svc_Conf_Lexer_Guard (const char *directive)
{
  // External synchronization is required.

  ::ace_yy_push_buffer (directive);
}


ACE_Svc_Conf_Lexer_Guard::~ACE_Svc_Conf_Lexer_Guard (void)
{
  // External synchronization is required.

  ::ace_yy_pop_buffer ();
}
