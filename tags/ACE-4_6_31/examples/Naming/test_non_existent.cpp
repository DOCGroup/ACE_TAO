// $Id$

#include "ace/Naming_Context.h"

ACE_RCSID(Naming, test_non_existent, "$Id$")

int main (int, char *[])
{
  int i;

  ACE_Naming_Context * ns_ptr = new ACE_Naming_Context ();
  ACE_Name_Options *name_options = ns_ptr->name_options ();

#if defined (ACE_WIN32)
  char * m_argv[] = {"MyName",
                     "-cNODE_LOCAL" ,
                     "-lC:\\temp\\non_existent",
                     NULL};
#else
  char * m_argv[] = {"MyName",
                     "-cNODE_LOCAL" ,
                     "-l/tmp/foobar.mine",
                     NULL};
#endif
  int m_argc = sizeof (m_argv) / sizeof (char *) -1;

  name_options->parse_args (m_argc,m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);
  ACE_DEBUG ((LM_DEBUG, "(%P) opened with %d\n", i));
  if (i != 0) return -1;

  i = ns_ptr->bind ("Key_Value","Val_Value","-");
  ACE_DEBUG ((LM_DEBUG, "(%P) bound with %d\n", i));

  return 0;
}
