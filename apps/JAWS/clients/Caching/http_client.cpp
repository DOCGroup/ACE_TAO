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

  ACE_OS::fprintf (stderr, "* ");
  while (ACE_OS::fgets (buf, sizeof (buf), stdin) != NULL)
    {
      char * s = buf;
      while (*s == ' ' || *s == '\t')
        s++;

      if (*s == '!')
        {
          // Shell command
          do s++; while (*s == ' ' || *s == '\t');
          if (ACE_OS::system (s) == -1)
            ACE_OS::fprintf (stderr, " ! Error executing: %s\n", s);
        }
      else if (ACE_OS::strncmp (s, "http://", 7) == 0)
        {
          // URL
          HTTP_Connector connector;
          connector.connect (s);
        }
      else
        ACE_OS::fprintf (stderr, " ? I don't understand: %s\n", s);

      ACE_OS::fprintf (stderr, "* ");
    }

  ACE_DEBUG ((LM_DEBUG, "\nBye!\n"));

  return 0;
}
