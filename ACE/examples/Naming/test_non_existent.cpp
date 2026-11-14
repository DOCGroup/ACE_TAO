#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"
#include <array>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context ns;

  ACE_Name_Options * const name_options = ns.name_options ();
  if (name_options == nullptr)
    return 1;

  ACE_TCHAR program_name[] = ACE_TEXT ("MyName");
  ACE_TCHAR context_arg[] = ACE_TEXT ("-cNODE_LOCAL");
#if defined (ACE_WIN32)
  ACE_TCHAR directory_arg[] = ACE_TEXT ("-lC:\\temp\\non_existent");
#else
  ACE_TCHAR directory_arg[] = ACE_TEXT ("-l/tmp/foobar.mine");
#endif /* ACE_WIN32 */
  std::array<ACE_TCHAR *, 4> m_argv {
    program_name, context_arg, directory_arg, nullptr
  };

  int const m_argc = static_cast<int> (m_argv.size () - 1);

  name_options->parse_args (m_argc, m_argv.data ());

  int const result = ns.open (ACE_Naming_Context::NODE_LOCAL);
  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              result));
  if (result != 0)
    return -1;

  int const bind_result = ns.bind ("Key_Value",
                                   "Val_Value",
                                   "-");

  ACE_DEBUG ((LM_DEBUG,
              "(%P) bound with %d\n",
              bind_result));
  return 0;
}
