// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/JAWS/clients/Caching
// 
// = FILENAME
//    http_client.cpp
//
// = DESCRIPTION
//    This is a very simple client.  It accepts URLs from a prompt, and
//    will try to fetch them.  Also accepts shell escapes.
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#include "http_handler.h"

ACE_RCSID(Caching, http_client, "$Id$")

int
main (int, char *[])
{
  // Present a command line.
  // * Accept a URL.
  //     Pass it to the HTTP_Connector.
  //     Connect.
  //     Report status.
  // * Accept shell escape character.

  char buf[BUFSIZ];

  ACE_DEBUG ((LM_DEBUG, "* "));

  while (ACE_OS::fgets (buf, sizeof (buf), stdin) != NULL)
    {
      char *s = buf;

      // get rid of trailing '\n'
      int len = ACE_OS::strlen (s);

      if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = 0;

      while (isspace (*s))
        s++;

      if (*s == '!')
        {
          do 
	    s++; 
	  while (isspace (*s));

          // Shell command.
          if (ACE_OS::system (s) == -1)
            ACE_ERROR ((LM_ERROR, " ! Error executing: %s\n", s));
        }
      else if (ACE_OS::strncmp (s, "http://", 7) == 0)
        {
          // URL
          HTTP_Connector connector;
          connector.connect (s);
        }
      else
        ACE_ERROR ((LM_ERROR, " ? I don't understand: %s\n", s));

      ACE_ERROR ((LM_ERROR, "* "));
    }

  ACE_DEBUG ((LM_DEBUG, "\nBye!\n"));

  return 0;
}
