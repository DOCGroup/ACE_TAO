// $Id$

#include "XML_Utils.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR *filename = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:"));
  ACE_TCHAR c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'f':
          filename = get_opt.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Usage: %s [-f <filename>]\n")
                             ACE_TEXT ("  -f: Specify the svcconf filename\n"),
                             argv[0]),
                            -1);
        }
    };

  if (filename == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "No filename specified\n"), -1);

  CIAO::Softpkg_Handler::Softpkg_Info info;
  info.csd_path_ = filename;

  if (CIAO::XML_Utils::parse_softpkg (&info) == 0)
    info.dump ();

  return 0;
}
