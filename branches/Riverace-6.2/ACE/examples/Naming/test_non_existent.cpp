// $Id$

#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"



int ACE_TMAIN (int, ACE_TCHAR *[])
{
  int i;

  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context *ns_ptr;
  ACE_NEW_RETURN (ns_ptr,
                  ACE_Naming_Context,
                  1);

  ACE_Name_Options *name_options = ns_ptr->name_options ();

  const ACE_TCHAR *m_argv[] =
  {
    ACE_TEXT("MyName"),
    ACE_TEXT("-cNODE_LOCAL") ,
#if defined (ACE_WIN32)
    ACE_TEXT("-lC:\\temp\\non_existent"),
#else
    ACE_TEXT("-l/tmp/foobar.mine"),
#endif /* ACE_WIN32 */
    0
  };

  int m_argc =
    sizeof (m_argv) / sizeof (ACE_TCHAR *) -1;

  name_options->parse_args (m_argc, (ACE_TCHAR**)m_argv);

  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);
  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              i));
  if (i != 0)
    return -1;

  i = ns_ptr->bind ("Key_Value",
                    "Val_Value",
                    "-");

  ACE_DEBUG ((LM_DEBUG,
              "(%P) bound with %d\n",
              i));
  return 0;
}
