//=============================================================================
/**
 *  @file    ContentHandler_Test.cpp
 *
 *  @author Steve Huston  <shuston@riverace.com>
 */
//=============================================================================

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/common/InputSource.h"
#include "ACEXML/common/LocatorImpl.h"
#include "ACEXML/common/AttributesImpl.h"
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
  ACEXML_InputSource input_source;
  input_source.setEncoding (ACE_TEXT ("encoding"));
  input_source.setPublicId (ACE_TEXT ("public-id"));
  input_source.setSystemId (ACE_TEXT ("system-id"));
  input_source.setEncoding (input_source.getEncoding ());
  input_source.setPublicId (input_source.getPublicId ());
  input_source.setSystemId (input_source.getSystemId ());

  if (input_source.getEncoding () == nullptr
      || input_source.getPublicId () == nullptr
      || input_source.getSystemId () == nullptr
      || std::char_traits<ACEXML_Char>::compare (input_source.getEncoding (),
                                                 ACE_TEXT ("encoding"),
                                                 8) != 0
      || std::char_traits<ACEXML_Char>::compare (input_source.getPublicId (),
                                                 ACE_TEXT ("public-id"),
                                                 9) != 0
      || std::char_traits<ACEXML_Char>::compare (input_source.getSystemId (),
                                                 ACE_TEXT ("system-id"),
                                                 9) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("InputSource self-copy failed\n")));
      return -1;
    }

  ACEXML_LocatorImpl locator (ACE_TEXT ("system-id"), ACE_TEXT ("public-id"));
  locator.setPublicId (locator.getPublicId ());
  locator.setSystemId (locator.getSystemId ());
  if (locator.getPublicId () == nullptr
      || locator.getSystemId () == nullptr
      || std::char_traits<ACEXML_Char>::compare (locator.getPublicId (),
                                              ACE_TEXT ("public-id"),
                                              9) != 0
      || std::char_traits<ACEXML_Char>::compare (locator.getSystemId (),
                                                 ACE_TEXT ("system-id"),
                                                 9) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("LocatorImpl self-copy failed\n")));
      return -1;
    }

  ACEXML_Attribute attribute (ACE_TEXT ("urn:"), ACE_TEXT ("local"),
                              ACE_TEXT ("p:local"), ACE_TEXT ("CDATA"),
                              ACE_TEXT ("value"));
  attribute.uri (attribute.uri ());
  attribute.localName (attribute.localName ());
  attribute.qName (attribute.qName ());
  attribute.type (attribute.type ());
  attribute.value (attribute.value ());
  ACEXML_Attribute attribute_copy (attribute);
  ACEXML_Attribute assigned_attribute;
  assigned_attribute = attribute_copy;
  if (assigned_attribute.uri () == nullptr
      || assigned_attribute.localName () == nullptr
      || assigned_attribute.qName () == nullptr
      || assigned_attribute.type () == nullptr
      || assigned_attribute.value () == nullptr
      || std::char_traits<ACEXML_Char>::compare (assigned_attribute.uri (),
                                              ACE_TEXT ("urn:"), 4) != 0
      || std::char_traits<ACEXML_Char>::compare (assigned_attribute.localName (),
                                                 ACE_TEXT ("local"), 5) != 0
      || std::char_traits<ACEXML_Char>::compare (assigned_attribute.qName (),
                                                 ACE_TEXT ("p:local"), 7) != 0
      || std::char_traits<ACEXML_Char>::compare (assigned_attribute.type (),
                                                 ACE_TEXT ("CDATA"), 5) != 0
      || std::char_traits<ACEXML_Char>::compare (assigned_attribute.value (),
                                                 ACE_TEXT ("value"), 5) != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Attribute self-copy failed\n")));
      return -1;
    }

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
