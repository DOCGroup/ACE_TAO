// $Id$

#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Svcconf_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR *filename = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("f:"));
  int c;

  while ((c = get_opt ()) != EOF)
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

    auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);

    ACEXML_Parser parser;
    ACEXML_InputSource input(stm);

    parser.setContentHandler (handler);
    parser.setDTDHandler (handler);
    parser.setErrorHandler (handler);
    parser.setEntityResolver (handler);

  ACEXML_TRY_NEW_ENV
    {
      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ex.print ();
      return -1;
  }
  ACEXML_ENDTRY;
  return 0;
}
