/**
 * $Id$
 *
 * ACE_Get_Opt long_only examples. Sample code from The ACE
 * Programmer's Guide, Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  static const ACE_TCHAR options[] = ACE_TEXT (":f:");
  ACE_Get_Opt cmd_opts
    (argc, argv, options, 1, 0, ACE_Get_Opt::PERMUTE_ARGS, 1);
  if (cmd_opts.long_option
      (ACE_TEXT ("config"), 'f', ACE_Get_Opt::ARG_REQUIRED) == -1)
    return -1;

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

    case ':':
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmd_opts.opt_opt ()), -1);


    default:
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config file is %s\n"), config_file));

  return 0;
}
