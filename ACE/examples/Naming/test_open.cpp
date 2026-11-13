#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include <array>

int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_TCHAR default_host[] = ACE_TEXT ("-hlocalhost");
  ACE_TCHAR default_port[] = ACE_TEXT ("-p20012");
  ACE_TCHAR *host = argc > 1 ? argv[1] : default_host;
  ACE_TCHAR *port = argc > 2 ? argv[2] : default_port;

  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context ns;
  ACE_Name_Options * const name_options = ns.name_options ();
  if (name_options == nullptr)
    return 1;

  ACE_TCHAR program_name[] = ACE_TEXT ("MyName");
  ACE_TCHAR context_arg[] = ACE_TEXT ("-cNET_LOCAL");
  std::array<ACE_TCHAR *, 5> m_argv {
    program_name, context_arg, host, port, nullptr
  };
  int const m_argc = static_cast<int> (m_argv.size () - 1);

  name_options->parse_args (m_argc, m_argv.data ());

  int const result = ns.open (ACE_Naming_Context::NET_LOCAL);
  ACE_DEBUG ((LM_DEBUG,
              "ACE_Naming_Context::open returned %d\n",
              result));
  if (result != 0)
    return result;

  char key[128];
  char val[32];
  char const type[] = "-";
  int const process_id = static_cast<int> (ACE_OS::getpid ());

  for (int l {1}; l <= 1000; ++l)
    {
      ACE_OS::sprintf (key,
                       "K_%05d_%05d",
                       process_id,
                       l);
      ACE_OS::sprintf (val,
                       "Val%05d",
                       l);
      int const bind_result = ns.bind (key, val, type);
      ACE_DEBUG ((LM_DEBUG,
                  "%d: bind of %s: %d\n",
                  process_id,
                  key,
                  bind_result));

      if (bind_result != 0)
        return -1;
    }

  int const close_result = ns.close ();
  ACE_DEBUG ((LM_DEBUG,
              "ACE_Naming_Context::close returned %d\n",
              close_result));
  return close_result;
}
