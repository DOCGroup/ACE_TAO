// $Id$

#include "ace/Naming_Context.h"

int main (int, char *[])
{
  int i;

  ACE_Naming_Context * ns_ptr = new ACE_Naming_Context ();
  ACE_Name_Options *name_options = ns_ptr->name_options ();

  char * m_argv[] = {"MyName",
                     "-cNODE_LOCAL",
                     NULL};

  int m_argc = sizeof (m_argv) / sizeof (char *) -1;

  name_options->parse_args (m_argc,m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);
  ACE_DEBUG ((LM_DEBUG, "(%P) opened with %d\n", i));
  if (i != 0) return -1;

  char Key[128];
  char Val[32];
  char Type[2];

  Type[0] = '-';
  Type[1] = '\0';

  for (int l = 1; l <= 1000 ; l++) {
    ACE_OS::sprintf (Key, "K_%05d_%05d", ACE_OS::getpid(), l);
    ACE_OS::sprintf (Val, "Val%05d", l);
    i = ns_ptr->bind (Key, Val, Type);
    ACE_DEBUG ((LM_DEBUG, "(%P) bind of %s: %d\n", Key, i));
    if (i != 0) {
      return -1;
    }
  }

  return 0;
}
