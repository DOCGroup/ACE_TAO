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
  Basic_Content_Tester (void) : status_ (0) {}

  /**
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length ACEXML_ENV_ARG_DECL)
        ACE_THROW_SPEC ((ACEXML_SAXException));

  const ACEXML_Char *get_test_string (void)
    { return Basic_Content_Tester::test_string_; }

  int get_status (void) { return this->status_; }

private:
  int status_;
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
{
  static int already_called = 0;
  static ACEXML_Char *expect =
    ACE_TEXT ("Example\nd'internationalisation");

  if (already_called)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Basic_Content_Tester::characters called too much\n")
                  ));
      return;
    }

  already_called = 1;

  int expected_len = ACE_static_cast (int, ACE_OS::strlen (expect));
  if (length != expected_len)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("characters() expected len %d (%*s); ")
                ACE_TEXT ("got %d (%*s)\n"),
                expected_len, expected_len, ch + start,
                length, length, ch + start));
  return;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Basic_Content_Tester tester;
  ACEXML_StrCharStream *test_stream =
    new ACEXML_StrCharStream (tester.get_test_string ());
  ACEXML_InputSource input (test_stream);
  ACEXML_Parser parser;
  parser.setContentHandler (&tester);
  parser.parse (&input);
  return tester.get_status ();
}
