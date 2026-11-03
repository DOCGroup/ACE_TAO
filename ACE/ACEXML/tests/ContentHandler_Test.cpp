//=============================================================================
/**
 *  @file    ContentHandler_Test.cpp
 *
 *  @author Steve Huston  <shuston@riverace.com>
 */
//=============================================================================

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ace/OS_main.h"
#include <memory>
#include <string>

class Basic_Content_Tester : public ACEXML_DefaultHandler
{
public:
  /**
   * Receive notification of character data.
   */
  void characters (ACEXML_Char const *ch,
                   size_t start,
                   size_t length) override;

  ACEXML_Char const *get_test_string () const
    { return Basic_Content_Tester::test_string_; }

private:
  static ACEXML_Char const *test_string_;
};

ACEXML_Char const * Basic_Content_Tester::test_string_ =
  ACE_TEXT ("<?xml version=\"1.0\"?>")
  ACE_TEXT ("<translation type=\"unfinished\">Example\n")
  ACE_TEXT ("d&apos;internationalisation</translation>");

void
Basic_Content_Tester::characters (ACEXML_Char const *ch,
                                  size_t start,
                                  size_t length)
{
  static bool already_called = false;
  static ACEXML_Char const *expect =
    ACE_TEXT ("Example\nd'internationalisation");

  if (already_called)
    {
      throw ACEXML_SAXException
                    (ACE_TEXT ("characters() called too much\n"));
    }
  already_called = true;

  size_t const expected_len = std::char_traits<ACEXML_Char>::length (expect);
  if (length != expected_len)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("characters() expected len %u (%*s); ")
                  ACE_TEXT ("got %u (%*s)\n"),
                  expected_len, expected_len, ch + start,
                  length, length, ch + start));
      throw ACEXML_SAXException (ACE_TEXT ("Functionality failure"));
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int status = 0;
  Basic_Content_Tester tester;
  ACEXML_StrCharStream *raw_test_stream = nullptr;
  ACE_NEW_RETURN (raw_test_stream, ACEXML_StrCharStream, -1);
  std::unique_ptr<ACEXML_StrCharStream> test_stream (raw_test_stream);
  if (test_stream->open (tester.get_test_string (),
                         ACE_TEXT ("test_stream")) < 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to create input stream\n")));
      return -1;
    }
  ACEXML_InputSource input (test_stream.release ());
  ACEXML_Parser parser;
  parser.setContentHandler (&tester);
  try
  {
    parser.setFeature (ACE_TEXT ("http://xml.org/sax/features/validation"),
                       false);
    parser.parse (&input);
  }
  catch (const ACEXML_SAXException& ex)
  {
    ex.print();
    status = 1;
  }
  return status;
}
