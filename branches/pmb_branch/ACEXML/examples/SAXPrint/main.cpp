// $Id$

#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Print_Handler.h"
#include "SAXPrint_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"

static const ACEXML_Char *test_string =
ACE_TEXT ("<?xml version='1.0'?> <ACE_Svc_Conf> <static id=\"ACE_Service_Manager\" params='-d -p 4911'/> <dynamic id=\"Test_Task\" type=\"service_object\"> &#65; &amp; <initializer path=\"CCM_App\" init='_make_Test_Task' params='-p 3000'/> </dynamic> </ACE_Svc_Conf>");

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s [-sl] [-f <filename> | -u <url>]\n")
              ACE_TEXT ("  -s: Use SAXPrint_Handler (Default is Print_Handler)\n")
              ACE_TEXT ("  -l: Parse the internal strings (test the StrCharStream class)\n")
              ACE_TEXT ("  -f: Specify the filename when -l is not specified\n")
              ACE_TEXT ("  -n: Use the \"Simple\" parsing feature\n")
              ACE_TEXT ("  -u: URL specifying the path to the file\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACEXML_Char* filename = 0;
  int sax = 0;                  // Use SAXPrint handler or not.
  int str = 0;
  int simple = 0;
  ACEXML_Char* url = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("snf:lu:"));
  int c;

  while ((c = get_opt ()) != EOF)
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
        case 'n':
          simple = 1;
          break;
        case 'u':
          url = get_opt.opt_arg();
          break;
        default:
          usage(argv[0]);
          return -1;
        }
    }

  if (str == 0 && filename == 0 && url == 0) {
    usage(argv[0]);
    return -1;
  }

  ACEXML_DefaultHandler *handler = 0;
  auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);
  ACEXML_CharStream *stm = 0;
  ACEXML_FileCharStream *fstm = 0;
  ACEXML_HttpCharStream *ustm = 0;
    if (filename != 0)
      {
        ACE_NEW_RETURN (fstm,
                        ACEXML_FileCharStream (),
                        -1);

        if (fstm->open (filename) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Failed to open XML file: %s\n"),
                             filename),
                            -1);
        stm = fstm;
      }
    else if (url != 0)
      {

        ACE_NEW_RETURN (ustm,
                        ACEXML_HttpCharStream (),
                        -1);
        if (ustm->open (url) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Failed to open URL : %s\n"),
                             url),
                            -1);
        stm = ustm;
      }
    else {
      ACE_NEW_RETURN (stm,
                        ACEXML_StrCharStream (test_string),
                        -1);
      }

  ACEXML_Char* name = (filename == 0) ? url : filename;
    if (sax == 0)
      ACE_NEW_RETURN (handler,
                    ACEXML_Print_Handler (name),
                      -1);
    else
      ACE_NEW_RETURN (handler,
                    ACEXML_SAXPrint_Handler (name),
                      -1);

  ACEXML_TRY_NEW_ENV
    {
      ACEXML_Parser parser;
      if (simple != 0)
        {
          parser.setFeature (ACE_TEXT ("Simple"), 1 ACEXML_ENV_ARG_PARAMETER);
          ACEXML_TRY_CHECK;
        }
      ACEXML_InputSource input(stm);

      parser.setContentHandler (handler);
      parser.setDTDHandler (handler);
      parser.setErrorHandler (handler);
      parser.setEntityResolver (handler);

      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCH (ACEXML_SAXException, ex)
    {
      ACE_UNUSED_ARG (ex);
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
  }
  ACEXML_ENDTRY;
  return 0;
}
