// $Id$

// Test the ACE_Get_Opt class.

#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"



int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("ab:cd:ef:gh:"));
  int c;

  while ((c = get_opt ()) != EOF)
    switch (c)
      {
      case 'a':
        ACE_DEBUG ((LM_DEBUG, "got a\n"));
        break;
      case 'b':
        ACE_DEBUG ((LM_DEBUG, "got b with arg %s\n", get_opt.opt_arg ()));
        break;
      case 'c':
        ACE_DEBUG ((LM_DEBUG, "got c\n"));
        break;
      case 'd':
        ACE_DEBUG ((LM_DEBUG, "got d with arg %s\n", get_opt.opt_arg ()));
        break;
      case 'e':
        ACE_DEBUG ((LM_DEBUG, "got e\n"));
        break;
      case 'f':
        ACE_DEBUG ((LM_DEBUG, "got f with arg %s\n", get_opt.opt_arg ()));
        break;
      case 'g':
        ACE_DEBUG ((LM_DEBUG, "got g\n"));
        break;
      case 'h':
        ACE_DEBUG ((LM_DEBUG, "got h with arg %s\n", get_opt.opt_arg ()));
        break;
      default:
        ACE_DEBUG ((LM_DEBUG, "got %c, which is unrecognized!\n", c));
        break;
      }

  for (int i = get_opt.opt_ind (); i < argc; i++)
    ACE_DEBUG ((LM_DEBUG, "optind = %d, argv[optind] = %s\n",
                i, argv[i]));

  return 0;
}
