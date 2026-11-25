#include "test_config.h"
#include "ace/XML_Utils/XMLSchema/Types.hpp"
#include "ace/XML_Utils/XSCRT/XML.hpp"
#include <xercesc/util/PlatformUtils.hpp>

#include <string>
#include <utility>
#include <vector>

namespace
{
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

    XSCRT::XML::string xml_string (std::string ("example"));
    ACE_TEST_ASSERT (XSCRT::XML::transcode<char> (xml_string.c_str (), nullptr) == "example");
    ACE_TEST_ASSERT (XSCRT::XML::transcode<char> (nullptr, nullptr).empty ());
  }
  xercesc::XMLPlatformUtils::Terminate ();
  ACE_END_TEST;
  return 0;
}
