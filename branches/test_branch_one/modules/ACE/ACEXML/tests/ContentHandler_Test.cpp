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
#include "ace/OS_NS_string.h"
#include "ace/OS_main.h"

class Basic_Content_Tester : public ACEXML_DefaultHandler
{
public:
  /**
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           size_t start,
                           size_t length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  const ACEXML_Char *get_test_string (void)
    { return Basic_Content_Tester::test_string_; }

private:
  static const ACEXML_Char *test_string_;
};

const ACEXML_Char * Basic_Content_Tester::test_string_ =
  ACE_TEXT ("<?xml version=\"1.0\"?>")
  ACE_TEXT ("<translation type=\"unfinished\">Example\n")
  ACE_TEXT ("d&apos;internationalisation</translation>");

void
Basic_Content_Tester::characters (const ACEXML_Char *ch,
                                  size_t start,
                                  size_t length ACEXML_ENV_ARG_DECL)
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

  size_t expected_len = ACE_OS::strlen (expect);
  if (length != expected_len)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("characters() expected len %u (%*s); ")
                  ACE_TEXT ("got %u (%*s)\n"),
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
  ACEXML_StrCharStream *test_stream = 0;
  ACE_NEW_RETURN (test_stream, ACEXML_StrCharStream, -1);
  if (test_stream->open (tester.get_test_string (),
                         ACE_TEXT ("test_stream")) < 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create input stream\n")));
      return -1;
    }
  ACEXML_InputSource input (test_stream);
  ACEXML_Parser parser;
  parser.setContentHandler (&tester);
  ACEXML_TRY_NEW_ENV
  {
    parser.setFeature (ACE_TEXT ("http://xml.org/sax/features/validation"),
                       0
                       ACEXML_ENV_ARG_PARAMETER);
    ACEXML_TRY_CHECK;
    parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
    ACEXML_TRY_CHECK;
  }
  ACEXML_CATCH (ACEXML_SAXException, ex)
  {
    ex.print();
    status = 1;
  }
  ACEXML_ENDTRY;
  return status;
}
