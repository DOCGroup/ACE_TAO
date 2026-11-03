#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/common/ZipCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "Print_Handler.h"
#include "SAXPrint_Handler.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include <memory>
#include <utility>

static ACEXML_Char const *test_string =
ACE_TEXT ("<?xml version='1.0'?> <ACE_Svc_Conf> <static id=\"ACE_Service_Manager\" params='-d -p 4911'/> <dynamic id=\"Test_Task\" type=\"service_object\"> &#65; &quot; <initializer path=\"CCM_App\" init='_make_Test_Task' params='-p 3000'/> </dynamic> </ACE_Svc_Conf>");

static void
usage (ACE_TCHAR const *program)
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
  ACEXML_Char* filename = nullptr;
  bool sax = false;             // Use SAXPrint handler or not.
  bool str = false;
#ifdef USE_ZZIP
  bool zip = false;
#endif
  ACEXML_Char* url = nullptr;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("sf:lu:z"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 's':
            sax = true;
            break;
          case 'l':
            str = true;
            break;
          case 'f':
            filename = get_opt.opt_arg ();
            break;
          case 'u':
            url = get_opt.opt_arg ();
            break;
          case 'z':
#ifndef USE_ZZIP
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ZZIPLIB support has not been")
                        ACE_TEXT (" compiled in. Refer to ")
                        ACE_TEXT ("$ACE_ROOT/ACEXML/README for more ")
                        ACE_TEXT ("information.\n")));
            return -1;
#else
            zip = true;
            break;
#endif /* USE_ZZIP */
          default:
            usage(argv[0]);
            return -1;
        }
    }

  if (!str && filename == nullptr && url == nullptr)
    {
      usage (argv[0]);
      return -1;
    }

  std::unique_ptr<ACEXML_DefaultHandler> handler;
  std::unique_ptr<ACEXML_CharStream> stm;
#ifdef USE_ZZIP
  std::unique_ptr<ACEXML_ZipCharStream> zstm;
#endif /* USE_ZZIP */
  if (filename != nullptr)
    {
#ifdef USE_ZZIP
      if (zip)
        {
          zstm = std::make_unique<ACEXML_ZipCharStream> ();
          if (zstm->open (filename) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Failed to open XML file: %s\n"),
                               filename),
                              -1);
          stm = std::move (zstm);
        }
      else
        {
#endif /* USE_ZZIP */
          std::unique_ptr<ACEXML_FileCharStream> fstm =
            std::make_unique<ACEXML_FileCharStream> ();
          if (fstm->open (filename) != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Failed to open XML file: %s\n"),
                               filename),
                              -1);
          stm = std::move (fstm);
#ifdef USE_ZZIP
        }
#endif /* USE_ZZIP */
    }
  else if (url != nullptr)
    {
      std::unique_ptr<ACEXML_HttpCharStream> ustm =
        std::make_unique<ACEXML_HttpCharStream> ();
      if (ustm->open (url) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to open URL : %s\n"),
                           url),
                          -1);
      stm = std::move (ustm);
    }
  else
    {
      std::unique_ptr<ACEXML_StrCharStream> sstm =
        std::make_unique<ACEXML_StrCharStream> ();
      if (sstm->open (test_string, ACE_TEXT ("test_string")) < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Failed to open string : %s\n"),
                           test_string),
                          -1);
      stm = std::move (sstm);
    }

  ACEXML_Char const *name = (filename == nullptr) ? url : filename;
  if (!sax)
    handler = std::make_unique<ACEXML_Print_Handler> (name);
  else
    handler = std::make_unique<ACEXML_SAXPrint_Handler> (name);

  ACEXML_Parser parser;
  ACEXML_InputSource input (stm.get ());
  stm.release (); // ACEXML_InputSource takes ownership of the stream.

  parser.setContentHandler (handler.get ());
  parser.setDTDHandler (handler.get ());
  parser.setErrorHandler (handler.get ());
  parser.setEntityResolver (handler.get ());

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
  return 0;
}

