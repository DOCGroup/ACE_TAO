// $Id$

#include "common/FileCharStream.h"
#include "common/StrCharStream.h"
#include "parser/parser/Parser.h"
#include "Print_Handler.h"
#include "SAXPrint_Handler.h"
#include "ace/Get_Opt.h"

static ACE_TCHAR *test_string =
ACE_TEXT ("<?xml version='1.0'?> <ACE_Svc_Conf> <static id=\"ACE_Service_Manager\" params='-d -p 4911'/> <dynamic id=\"Test_Task\" type=\"service_object\"> &#65; &amp; <initializer path=\"CCM_App\" init='_make_Test_Task' params='-p 3000'/> </dynamic> </ACE_Svc_Conf>");


int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR *filename = 0;
  int sax = 0;                  // Use SAXPrint handler or not.
  int str = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("sf:l"));
  ACE_TCHAR c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 's':
          sax = 1;
          break;
        case 'l':
          str = 1;
          break;
        case 'f':
          filename = get_opt.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Usage: %s [-sl] [-f <filename>]\n")
                             ACE_TEXT ("  -s: Use SAXPrint_Handler (Default is Print_Handler\n")
                             ACE_TEXT ("  -l: Parse the internal strings (test the StrCharStream class\n")
                             ACE_TEXT ("  -f: Specify the filename when -l is not specified\n"),
                             argv[0]),
                            -1);
        }
    };

  if (str == 0 && filename == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "No filename specified\n"), -1);

  ACEXML_DefaultHandler *handler = 0;
  {
    ACEXML_CharStream *stm = 0;
    if (str == 0)
      {
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
      }
    else
      {
        ACE_NEW_RETURN (stm,
                        ACEXML_StrCharStream (test_string),
                        1);
      }

    if (sax == 0)
      ACE_NEW_RETURN (handler,
                      ACEXML_Print_Handler (),
                      -1);
    else
      ACE_NEW_RETURN (handler,
                      ACEXML_SAXPrint_Handler (),
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
