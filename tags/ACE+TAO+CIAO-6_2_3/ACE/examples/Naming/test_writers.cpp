// $Id$

#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  int i;

  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context *ns_ptr;
  ACE_NEW_RETURN (ns_ptr,
                  ACE_Naming_Context,
                  1);
  ACE_Name_Options *name_options =
    ns_ptr->name_options ();

  const ACE_TCHAR *m_argv[] =
  {
    ACE_TEXT("MyName"),
    ACE_TEXT("-cNODE_LOCAL"),
    0
  };

  int m_argc = sizeof (m_argv) / sizeof (ACE_TCHAR *) -1;

  name_options->parse_args (m_argc,
                            (ACE_TCHAR **) m_argv);
  i = ns_ptr->open (ACE_Naming_Context::NODE_LOCAL);
  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              i));
  if (i != 0)
    return -1;
  else
    {
      char key[128];
      char val[32];
      char type[2];

      type[0] = '-';
      type[1] = '\0';

      int i = 0;

      for (int l = 1; l <= 1000 ; l++)
        {
          ACE_OS::sprintf (key,
                           "K_%05d_%05d",
                           (int) ACE_OS::getpid (),
                           l);
          ACE_OS::sprintf (val,
                           "Val%05d",
                           l);
          i = ns_ptr->bind (key,
                            val,
                            type);
          ACE_DEBUG ((LM_DEBUG,
                      "%d: bind of %s: %d\n",
                      ACE_OS::getpid (),
                      key,
                      i));

          if (i != 0)
            return -1;

        }
    }
  return 0;
}
