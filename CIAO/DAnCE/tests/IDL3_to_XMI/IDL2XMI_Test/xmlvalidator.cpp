// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include "XML_Helper.h"

const ACE_TCHAR *inp_file = ACE_TEXT ("");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        inp_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-i <inputfile>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0)
    {
      return 1;
    }

  DAnCE::XML::XML_Error_Handler handler;
  Test::XML::Helper helper (handler);

  if (0 != helper.validate_dom (inp_file))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error: %s - invalid xml\n"),
                  inp_file));
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s - valid xml\n"),
              inp_file));

  return 0;
}
