/**
 * $Id$
 *
 * ACE_Get_Opt examples not in a larger program. Sample code from The ACE
 * Programmer's Guide, Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  // Example for a long option without a corresponding short option.
  // Just put some context here so the following compiles and runs.
  static const ACE_TCHAR options[] = ACE_TEXT (":f:");
  ACE_Get_Opt cmd_opts (argc, argv, options);

  // Listing 1 code/ch04
  cmd_opts.long_option (ACE_TEXT ("cool_option"));
  cmd_opts.long_option (ACE_TEXT ("the_answer"), 42);
  // Listing 1

  int option;
  ACE_TCHAR config_file[MAXPATHLEN];
  ACE_OS_String::strcpy (config_file, ACE_TEXT ("HAStatus.conf"));
  while ((option = cmd_opts ()) != EOF)
    switch (option) {
    case 'f':
      ACE_OS_String::strncpy (config_file,
                              cmd_opts.opt_arg (),
                              MAXPATHLEN);
      break;

      // Listing 2 code/ch04
    case 0:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Yes, very cool.\n")));
      break;

    case 42:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("the_answer is 42\n")));
      break;
      // Listing 2

    case ':':
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmd_opts.opt_opt ()), -1);


    default:
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }

  return 0;
}
