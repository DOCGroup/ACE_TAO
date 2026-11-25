#include "test_config.h"
#include "ace/XML_Utils/XMLSchema/Types.hpp"
#include "ace/XML_Utils/XMLSchema/Writer.hpp"
#include "ace/XML_Utils/XML_Helper.h"
#include "ace/OS_NS_unistd.h"
#include "ace/XML_Utils/XSCRT/XML.hpp"
#include <xercesc/util/PlatformUtils.hpp>

#include <string>
#include <utility>
#include <vector>

namespace
{
  // Match XSC-generated writers: the derived writer default-constructs its
  // intermediate base while initializing their shared virtual writer base.
  class Generated_Base_Writer : public virtual XSCRT::Writer<char>
  {
  protected:
    Generated_Base_Writer () = default;
  };

  class Generated_Derived_Writer : public Generated_Base_Writer
  {
  public:
    explicit Generated_Derived_Writer (XSCRT::XML::Element<char> const &element)
      : XSCRT::Writer<char> (element)
    {
    }
  };

  class Type_Probe : public XSCRT::Type
  {
  public:
    Type_Probe () = default;

    Type_Probe (Type_Probe &&other)
      : XSCRT::Type (std::move (other))
    {
    }

    Type_Probe &operator= (Type_Probe &&other)
    {
      XSCRT::Type::operator= (std::move (other));
      return *this;
    }
  };
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("XML_Utils_Test"));
  xercesc::XMLPlatformUtils::Initialize ();
  {
    XMLSchema::ID<char> id (std::string ("first"));
    std::vector<XMLSchema::ID<char>> ids;
    ids.push_back (std::move (id));
    ACE_TEST_ASSERT (id == "first");
    ids.push_back (XMLSchema::ID<char> (std::string ("second")));
    ids[0] = XMLSchema::ID<char> (std::string ("third"));
    ACE_TEST_ASSERT (ids[0] == "third" && ids[1] == "second");

    XMLSchema::IDREF<char> ref (std::string ("first"));
    std::vector<XMLSchema::IDREF<char>> refs;
    refs.push_back (std::move (ref));
    ACE_TEST_ASSERT (ref.id () == "first");
    refs.push_back (XMLSchema::IDREF<char> (std::string ("second")));
    refs[0] = XMLSchema::IDREF<char> (std::string ("third"));
    ACE_TEST_ASSERT (refs[0].id () == "third" && refs[1].id () == "second");

    Type_Probe first;
    Type_Probe second (std::move (first));
    first = std::move (second);

    XML::XStr text (ACE_TEXT ("hello"));
    XML::XStr const suffix (ACE_TEXT (" world"));
    ACE_TEST_ASSERT (text.append (suffix.begin ()));
    ACE_TEST_ASSERT (text == XML::XStr (ACE_TEXT ("hello world")));
    ACE_TEST_ASSERT (text.erase (text.begin () + 5, text.end ()));
    ACE_TEST_ASSERT (text == XML::XStr (ACE_TEXT ("hello")));
    XML::XStr copy (text);
    text = suffix;
    ACE_TEST_ASSERT (copy == XML::XStr (ACE_TEXT ("hello")));
    XML::XStr moved (std::move (copy));
    ACE_TEST_ASSERT (copy.begin () == nullptr);
    copy = std::move (moved);
    ACE_TEST_ASSERT (moved.begin () == nullptr);
    XMLCh *released = copy.release ();
    ACE_TEST_ASSERT (copy.begin () == nullptr && released != nullptr);
    ACE_TEST_ASSERT (xercesc::XMLString::equals (released, XML::XStr (ACE_TEXT ("hello"))));
    xercesc::XMLString::release (&released);

    {
      XML::XML_Helper<> helper;
      ACE_TCHAR const *const filename = ACE_TEXT ("XML_Utils_Test.xml");
      xercesc::DOMDocument *document = helper.create_dom (ACE_TEXT ("root"), ACE_TEXT ("urn:test"));
      ACE_TEST_ASSERT (document != nullptr);
      xercesc::DOMElement *const document_root = document->getDocumentElement ();
      ACE_TEST_ASSERT (document_root != nullptr);
      XSCRT::XML::Element<char> root (document_root);
      XMLSchema::Writer::IDREF<char> writer (root);
      XMLSchema::IDREF<char> const value (std::string ("reference"));
      writer.traverse (value);
      ACE_TEST_ASSERT (helper.write_DOM (document, filename));
      document->release ();
      xercesc::XercesDOMParser parser;
      parser.setValidationScheme (xercesc::AbstractDOMParser::Val_Never);
      parser.parse (ACE_TEXT_ALWAYS_CHAR (filename));
      xercesc::DOMDocument *parsed = parser.getDocument ();
      ACE_TEST_ASSERT (parsed != nullptr);
      xercesc::DOMElement *const parsed_root = parsed->getDocumentElement ();
      ACE_TEST_ASSERT (parsed_root != nullptr);
      ACE_TEST_ASSERT (xercesc::XMLString::equals (
        parsed_root->getTagName (), XML::XStr (ACE_TEXT ("root"))));
      ACE_TEST_ASSERT (xercesc::XMLString::equals (
        parsed_root->getTextContent (), XML::XStr (ACE_TEXT ("reference"))));
      ACE_TEST_ASSERT (ACE_OS::unlink (filename) == 0);
    }

    XSCRT::XML::string xml_string (std::string ("example"));
    ACE_TEST_ASSERT (XSCRT::XML::transcode<char> (xml_string.c_str (), nullptr) == "example");
    ACE_TEST_ASSERT (XSCRT::XML::transcode<char> (nullptr, nullptr).empty ());
  }
  xercesc::XMLPlatformUtils::Terminate ();
  ACE_END_TEST;
  return 0;
}
