// $Id$

#include "common/FileCharStream.h"
#include "common/StrCharStream.h"
#include "parser/parser/Parser.h"
#include "Svcconf_Handler.h"
#include "ace/Get_Opt.h"

int
main (int argc, ACE_TCHAR *argv[])
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

  ACEXML_DefaultHandler *handler = 0;
  {
    ACEXML_CharStream *stm = 0;
    ACEXML_FileCharStream *fstm = 0;
    ACE_NEW_RETURN (fstm,
                    ACEXML_FileCharStream (),
                    1);

    if (fstm->open (filename) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Fail to open XML file: %s\n"),
                         filename),
                        -1);
    stm = fstm;

    ACE_NEW_RETURN (handler,
                    ACEXML_Svcconf_Handler (),
                    -1);

    ACEXML_Parser parser;
    ACEXML_InputSource input(stm);

    parser.setContentHandler (handler);
    parser.setDTDHandler (handler);
    parser.setErrorHandler (handler);
    parser.setEntityResolver (handler);

    ACEXML_Env xmlenv;

    parser.parse (&input, xmlenv);
    if (xmlenv.exception ())
      xmlenv.exception ()->print ();
  }
  delete handler;
  return 0;
}
