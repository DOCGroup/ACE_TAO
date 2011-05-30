// $Id$

#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/ZipCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Print_Handler.h"
#include "SAXPrint_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

static const ACEXML_Char *test_string =
ACE_TEXT ("<?xml version='1.0'?> <ACE_Svc_Conf> <static id=\"ACE_Service_Manager\" params='-d -p 4911'/> <dynamic id=\"Test_Task\" type=\"service_object\"> &#65; &quot; <initializer path=\"CCM_App\" init='_make_Test_Task' params='-p 3000'/> </dynamic> </ACE_Svc_Conf>");

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s [-sl] [-f <filename> | -u <url> | -z <ZIP Archive>]\n")
              ACE_TEXT ("  -s: Use SAXPrint_Handler (Default is Print_Handler)\n")
              ACE_TEXT ("  -l: Parse the internal strings (test the StrCharStream class)\n")
              ACE_TEXT ("  -f: Specify the filename when -l is not specified\n")
              ACE_TEXT ("  -z: Specify that the file is inside a ZIP archive\n")
              ACE_TEXT ("  -u: URL specifying the path to the file\n"),
              program));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACEXML_Char* filename = 0;
  int sax = 0;                  // Use SAXPrint handler or not.
  int str = 0;
  ACEXML_Char* url = 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("sf:lu:z"));
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
          case 'u':
            url = get_opt.opt_arg();
            break;
          case 'z':
            zip = 1;
#ifndef USE_ZZIP
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ZZIPLIB support has not been")
                        ACE_TEXT (" compiled in. Refer to ")
                        ACE_TEXT ("$ACE_ROOT/ACEXML/README for more ")
                        ACE_TEXT ("information.\n")));
            return -1;
#else
            break;
#endif /* USE_ZZIP */
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
  ACEXML_CharStream *stm = 0;
  ACEXML_FileCharStream *fstm = 0;
  ACEXML_HttpCharStream *ustm = 0;
  ACEXML_StrCharStream* sstm = 0;
#ifdef USE_ZZIP
  ACEXML_ZipCharStream* zstm = 0;
  int zip = 0;
#endif /* USE_ZZIP */
  if (filename != 0)
    {
#ifdef USE_ZZIP
      if (zip)
        {
          ACE_NEW_RETURN (zstm, ACEXML_ZipCharStream(), -1);
          if (zstm->open (filename) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Failed to open XML file: %s\n"),
                               filename),
                              -1);
          stm = zstm;
        }
      else
        {
#endif /* USE_ZZIP */
          ACE_NEW_RETURN (fstm, ACEXML_FileCharStream (), -1);
          if (fstm->open (filename) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Failed to open XML file: %s\n"),
                               filename),
                              -1);
          stm = fstm;
#ifdef USE_ZZIP
        }
#endif /* USE_ZZIP */
    }
  else if (url != 0)
    {

      ACE_NEW_RETURN (ustm, ACEXML_HttpCharStream (), -1);
      if (ustm->open (url) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to open URL : %s\n"),
                           url),
                          -1);
      stm = ustm;
    }
  else
    {
      ACE_NEW_RETURN (sstm, ACEXML_StrCharStream, -1);
      if (sstm->open (test_string, ACE_TEXT ("test_string")) < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to open string : %s\n"),
                           test_string),
                          -1);
      stm = sstm;
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
  auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);

  ACEXML_Parser parser;
  ACEXML_InputSource input (stm);

  parser.setContentHandler (handler);
  parser.setDTDHandler (handler);
  parser.setErrorHandler (handler);
  parser.setEntityResolver (handler);

  try
  {
    parser.parse (&input);
  }
  catch (const ACEXML_Exception& ex)
    {
      ex.print();
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
    }
  try
  {
    parser.parse (&input);
  }
  catch (const ACEXML_SAXException& ex)
    {
      ex.print();
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
      return 1;
    }
//   ACEXML_TRY_EX (THIRD)
//   {
//     parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
//     ACEXML_TRY_CHECK_EX (THIRD);
//   }
//   ACEXML_CATCH (ACEXML_SAXException, ex)
//     {
//       ex.print();
//       ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
//       return 1;
//     }
//   ACEXML_ENDTRY;
//   ACEXML_TRY_EX (FOURTH)
//   {
//     parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
//     ACEXML_TRY_CHECK_EX (FOURTH);
//   }
//   ACEXML_CATCH (ACEXML_SAXException, ex)
//     {
//       ex.print();
//       ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
//       return 1;
//     }
//   ACEXML_ENDTRY;
  return 0;
}

