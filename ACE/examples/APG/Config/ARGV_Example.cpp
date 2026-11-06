/**
 * ACE_ARGV examples not in a larger program. Sample code from The ACE
 * Programmer's Guide, Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include <string>

#include "ace/ARGV.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

struct Command_Line_Options
{
  std::basic_string<ACE_TCHAR> config_file {ACE_TEXT ("HAStatus.conf")};
  std::basic_string<ACE_TCHAR> hostname {ACE_TEXT ("not set")};
};

// Listing 1 code/ch04
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  static constexpr ACE_TCHAR option_string[] = ACE_TEXT (":f:h:");
  static constexpr ACE_TCHAR cmdline[] =
    ACE_TEXT ("-f /home/managed.cfg -h $HOSTNAME");
  ACE_ARGV cmdline_args {cmdline};
  ACE_Get_Opt cmd_opts (cmdline_args.argc (),
                        cmdline_args.argv (),
                        option_string,
                        0);          // Don't skip any args

// Listing 1

  int option {};
  Command_Line_Options command_line {};
  while ((option = cmd_opts ()) != EOF)
    switch (option)
      {
      case 'f':
        command_line.config_file = cmd_opts.opt_arg ();
        break;

      case 'h':
        command_line.hostname = cmd_opts.opt_arg ();
        break;

      case ':':
        ACE_ERROR_RETURN
          ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
            cmd_opts.opt_opt ()), -1);

      default:
        ACE_ERROR_RETURN
          ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
      }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config file: %s\n"),
              command_line.config_file.c_str ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Hostname: %s\n"),
              command_line.hostname.c_str ()));
  return 0;
}
