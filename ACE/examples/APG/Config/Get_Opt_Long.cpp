/**
 * ACE_Get_Opt long_only examples. Sample code from The ACE
 * Programmer's Guide, Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include <string>

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  static constexpr ACE_TCHAR options[] = ACE_TEXT (":f:");
  ACE_Get_Opt cmd_opts {argc, argv, options, 1, 0, ACE_Get_Opt::PERMUTE_ARGS, 1};
  if (cmd_opts.long_option
      (ACE_TEXT ("config"), 'f', ACE_Get_Opt::ARG_REQUIRED) == -1)
    return -1;

  int option {};
  std::basic_string<ACE_TCHAR> config_file {ACE_TEXT ("HAStatus.conf")};
  while ((option = cmd_opts ()) != EOF)
    switch (option)
      {
      case 'f':
        config_file = cmd_opts.opt_arg ();
        break;

      case ':':
        ACE_ERROR_RETURN
          ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
            cmd_opts.opt_opt ()), -1);

      default:
        ACE_ERROR_RETURN
          ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
      }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config file is %s\n"), config_file.c_str ()));

  return 0;
}
