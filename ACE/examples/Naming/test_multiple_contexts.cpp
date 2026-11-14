#include "ace/OS_main.h"
#include "ace/Naming_Context.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include <array>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  static constexpr u_long ACE_DEFAULT_BASE_ADDR_1 = (1 * 64 * 1024 * 1024);
  static constexpr u_long ACE_DEFAULT_BASE_ADDR_2 = (2 * 64 * 1024 * 1024);

  ACE_STATIC_SVC_REGISTER(ACE_Naming_Context);

  ACE_Naming_Context ns;
  ACE_Name_Options * const name_options = ns.name_options ();
  if (name_options == nullptr)
    return 1;

  ACE_Naming_Context ns1;
  ACE_Name_Options * const name_options1 = ns1.name_options ();
  if (name_options1 == nullptr)
    return 1;

  ACE_TCHAR address_arg1[BUFSIZ];
  ACE_TCHAR address_arg2[BUFSIZ];
  ACE_OS::sprintf (address_arg1,
                   ACE_TEXT("-b%ld"),
                   ACE_DEFAULT_BASE_ADDR_1);

  ACE_TCHAR program_name1[] = ACE_TEXT ("MyName1");
  ACE_TCHAR context_arg1[] = ACE_TEXT ("-cNODE_LOCAL");
  std::array<ACE_TCHAR *, 4> m_argv {
    program_name1, context_arg1, address_arg1, nullptr
  };
  int const m_argc = static_cast<int> (m_argv.size () - 1);

  ACE_OS::sprintf (address_arg2,
                   ACE_TEXT("-b%ld"),
                   ACE_DEFAULT_BASE_ADDR_2);
  ACE_TCHAR program_name2[] = ACE_TEXT ("MyName2");
  ACE_TCHAR context_arg2[] = ACE_TEXT ("-cNODE_LOCAL");
  std::array<ACE_TCHAR *, 4> n_argv {
    program_name2, context_arg2, address_arg2, nullptr
  };
  int const n_argc = static_cast<int> (n_argv.size () - 1);

  name_options->parse_args (m_argc, m_argv.data ());
  int const result = ns.open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) opened with %d\n",
              result));

  if (result != 0)
    return -1;

  name_options1->parse_args (n_argc, n_argv.data ());

  int const result1 = ns1.open (ACE_Naming_Context::NODE_LOCAL);

  ACE_DEBUG ((LM_DEBUG,
              "(%P) 1 opened with %d\n",
              result1));
  if (result1 != 0)
    return -1;

  return 0;
}
