//=============================================================================
/**
 *  @file    ContentHandler_Test.cpp
 *
 *  $Id$
 *
 *  @author Steve Huston  <shuston@riverace.com>
 */
//=============================================================================

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"

class Basic_Content_Tester : public ACEXML_DefaultHandler
{
public:
  /**
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  const ACEXML_Char *get_test_string (void)
    { return Basic_Content_Tester::test_string_; }

private:
  static const ACEXML_Char *test_string_;
};

const ACEXML_Char * Basic_Content_Tester::test_string_ =
  ACE_TEXT ("<?xml version=\"1.0\"?>")
  ACE_TEXT ("<translation type=\"unfinished\">Example\n")
  ACE_TEXT ("d&apos;internationalisation</translation></xml>");

void
Basic_Content_Tester::characters (const ACEXML_Char *ch,
                                  int start,
                                  int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException))
{
  static int already_called = 0;
  static const ACEXML_Char *expect =
    ACE_TEXT ("Example\nd'internationalisation");

  if (already_called)
    {
      ACEXML_THROW (ACEXML_SAXException
                    (ACE_TEXT ("characters() called too much\n")));
    }
  already_called = 1;

  int expected_len = ACE_static_cast (int, ACE_OS::strlen (expect));
  if (length != expected_len)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("characters() expected len %d (%*s); ")
                  ACE_TEXT ("got %d (%*s)\n"),
                  expected_len, expected_len, ch + start,
                  length, length, ch + start));
      ACEXML_THROW (ACEXML_SAXException (ACE_TEXT ("Functionality failure")));
    }
  return;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int status = 0;
  Basic_Content_Tester tester;
  ACEXML_StrCharStream *test_stream =
    new ACEXML_StrCharStream (tester.get_test_string ());
  ACEXML_InputSource input (test_stream);
  ACEXML_Parser parser;
  parser.setContentHandler (&tester);
  ACEXML_TRY_NEW_ENV
    {
      parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
      ACEXML_TRY_CHECK;
    }
  ACEXML_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Caught exception.\n")));
      status = 1;
    }
  ACEXML_ENDTRY;
  return status;
}
