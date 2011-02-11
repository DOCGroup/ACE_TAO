// $Id$

#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
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

  ACE_TCHAR address_arg1[BUFSIZ];
  ACE_TCHAR address_arg2[BUFSIZ];
  ACE_OS::sprintf (address_arg1,
                   ACE_TEXT("-b%ld"),
                   ACE_DEFAULT_BASE_ADDR_1);

  const ACE_TCHAR *m_argv[] =
  {
    ACE_TEXT("MyName1"),
    ACE_TEXT("-cNODE_LOCAL"),
    address_arg1,
    0
  };
  int m_argc =
    sizeof (m_argv) / sizeof (ACE_TCHAR *) -1;

  ACE_OS::sprintf (address_arg2,
                   ACE_TEXT("-b%ld"),
                   ACE_DEFAULT_BASE_ADDR_2);
  const ACE_TCHAR *n_argv[] =
  {
    ACE_TEXT("MyName2"),
    ACE_TEXT("-cNODE_LOCAL"),
    address_arg2,
    0
  };

  int n_argc =
    sizeof (n_argv) / sizeof (ACE_TCHAR *) -1;

  name_options->parse_args (m_argc,
                            (ACE_TCHAR **) m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              i));

  if (i != 0)
    return -1;

  name_options1->parse_args (n_argc,
                             (ACE_TCHAR **) n_argv);

  i = ns_ptr1->open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) 1 opened with %d\n",
              i));
  if (i != 0)
    return -1;

  return 0;
}
