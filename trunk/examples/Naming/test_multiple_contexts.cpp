// $Id$

#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"

ACE_RCSID(Naming, test_multiple_contexts, "$Id$")

int main (int, char *[])
{
  static u_long ACE_DEFAULT_BASE_ADDR_1 = (1 * 64 * 1024 * 1024);
  static u_long ACE_DEFAULT_BASE_ADDR_2 = (2 * 64 * 1024 * 1024);

  int i;

  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context *ns_ptr;
  ACE_NEW_RETURN (ns_ptr,
                  ACE_Naming_Context,
                  1);
  ACE_Name_Options *name_options =
    ns_ptr->name_options ();

  ACE_Naming_Context *ns_ptr1;
  ACE_NEW_RETURN (ns_ptr1,
                  ACE_Naming_Context,
                  1);
  ACE_Name_Options *name_options1 =
    ns_ptr1->name_options ();

  char address_arg1[BUFSIZ];
  char address_arg2[BUFSIZ];
  ACE_OS::sprintf (address_arg1,
                   "-b%d",
                   ACE_DEFAULT_BASE_ADDR_1);

  const char *m_argv[] = 
  {
    "MyName1",
    "-cNODE_LOCAL",
    address_arg1,
    NULL
  };
  int m_argc =
    sizeof (m_argv) / sizeof (char *) -1;

  ACE_OS::sprintf (address_arg2,
                   "-b%d",
                   ACE_DEFAULT_BASE_ADDR_2);
  const char *n_argv[] =
  {
    "MyName2",
    "-cNODE_LOCAL",
    address_arg2,
    NULL
  };

  int n_argc =
    sizeof (n_argv) / sizeof (char *) -1;

  name_options->parse_args (m_argc,
                            (char **) m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              i));

  if (i != 0) 
    return -1;

  name_options1->parse_args (n_argc,
                             (char **) n_argv);

  i = ns_ptr1->open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) 1 opened with %d\n",
              i));
  if (i != 0) 
    return -1;

  return 0;
}
