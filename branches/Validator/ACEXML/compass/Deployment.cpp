// $Id$

#include "ACEXML/common/XML_Types.h"
#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/compass/SoftPkgHandler.h"
#include "ace/Get_Opt.h"

static void
usage (const ACE_TCHAR* program)
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("Usage: %s [-l <filename>]")
              ACE_TEXT ("  -l: Install components from filename")
              program));
}

int
ACE_TMAIN (int argc, char* argv[])
{
  ACEXML_Char* filename = 0;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("l:"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
      switch (c)
        {
          case 'l':
            filename = get_opt.opt_arg ();
            break;
          default:
            usage(argv[0]);
            return -1;
        }
    }
  if (filename == 0)
    {
      usage(argv[0]);
      return -1;
    }
  const ACEXML_Char meta-inf[] = {'m', 'e', 't', 'a', '-', 'i', 'n', 'f'};
  const ACEXML_Char* path = 0;
  size_t pos = ACE_OS::strlen (filename);
  if ((ACEXML_Char* temp = ACE_OS::strrchr (filename, '.')) != 0)
    pos = temp - filename + 1;
  size_t len = pos + sizeof (meta-inf) +
               sizeof (ACE_DIRECTORY_SEPARATOR_CHAR) + sizeof (ACE_TCHAR);
  ACE_NEW_RETURN (path, ACEXML_Char[len], -1);
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_path (path);

  ACE_OS::strncpy (path, filename, pos);
  path[pos++] = ACE_DIRECTORY_SEPARATOR_CHAR;
  ACE_OS::strcpy (path + pos, meta-inf);

  ACEXML_StreamFactory factory;
  ACEXML_CharStream* stream = factory.create_stream (path);
  if (!stream)
    {
      ACE_ERROR ((LM_ERROR, "Failed to create input stream from %s\n",
                  path));
      return -1;
    }

  ACEXML_DefaultHandler* handler = 0;
  ACE_NEW_RETURN (handler, SoftPkgHandler (name), -1);
  auto_ptr<ACEXML_DefaultHandler> cleanup_handler (handler);

  ACEXML_Parser parser;
  ACEXML_InputSource input (stm);

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
      ACE_UNUSED_ARG (ex);
      ACE_DEBUG ((LM_ERROR, ACE_TEXT ("Exception occurred. Exiting...\n")));
    }
  ACEXML_ENDTRY;
  return 0;
}
