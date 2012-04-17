/**
 * $Id$
 *
 * ACE_ARGV examples not in a larger program. Sample code from The ACE
 * Programmer's Guide, Copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#include "ace/os_include/os_netdb.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch04
#include "ace/ARGV.h"
#include "ace/Get_Opt.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  static const ACE_TCHAR options[] = ACE_TEXT (":f:h:");
  static const ACE_TCHAR cmdline[] =
    ACE_TEXT ("-f /home/managed.cfg -h $HOSTNAME");
  ACE_ARGV cmdline_args (cmdline);
  ACE_Get_Opt cmd_opts (cmdline_args.argc (),
                        cmdline_args.argv (),
                        options,
                        0);          // Don't skip any args

// Listing 1

  int option;
  ACE_TCHAR config_file[MAXPATHLEN];
  ACE_TCHAR hostname[MAXHOSTNAMELEN];
  ACE_OS_String::strcpy (config_file, ACE_TEXT ("HAStatus.conf"));
  ACE_OS_String::strcpy (hostname, ACE_TEXT ("not set"));
  while ((option = cmd_opts ()) != EOF)
    switch (option) {
    case 'f':
      ACE_OS_String::strncpy (config_file,
                              cmd_opts.opt_arg (),
                              MAXPATHLEN);
      break;

    case 'h':
      ACE_OS_String::strncpy (hostname,
                              cmd_opts.opt_arg (),
                              MAXHOSTNAMELEN);
      break;

    case ':':
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("-%c requires an argument\n"),
          cmd_opts.opt_opt ()), -1);


    default:
      ACE_ERROR_RETURN
        ((LM_ERROR, ACE_TEXT ("Parse error.\n")), -1);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Config file: %s\n"), config_file));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Hostname: %s\n"), hostname));
  return 0;
}
