// file      : XSCRT/Parser.ipp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace XSCRT
{
  /*

  //@@ recode to delegate to one function.
  //
  template<>
  inline
  std::wstring
  parse<std::wstring> (XML::Element const& e, Parser&, std::wstring*)
  {
    return XML::transcode (e.dom_element ()->getTextContent ());
  }

  template<>
  inline
  std::wstring
  parse<std::wstring> (XML::Attribute const& a, Parser&, std::wstring*)
  {
    return a.value ();
  }

  template<>
  inline
  XMLSchema::byte
  parse<XMLSchema::byte> (XML::Element const& e, Parser& p, XMLSchema::byte*)
  {
    wchar_t r;
    std::wstringstream s;
    s << parse<std::wstring> (e, p, 0);
    s >> r;

    return static_cast<char>(r);
  }

  template<>
  inline
  XMLSchema::byte
  parse<XMLSchema::byte> (XML::Attribute const& a, Parser&, XMLSchema::byte*)
  {
    wchar_t r;
    std::wstringstream s;
    s << a.value ();
    s >> r;

    return static_cast<char>(r);
  }

  template<>
  inline
  XMLSchema::unsignedByte
  parse<XMLSchema::unsignedByte> (XML::Element const& e,
                                  Parser& p,
                                  XMLSchema::unsignedByte*)
  {
    wchar_t r;
    std::wstringstream s;
    s << parse<std::wstring> (e, p, 0);
    s >> r;

    return static_cast<unsigned char>(r);
  }

  template<>
  inline
  XMLSchema::unsignedByte
  parse<XMLSchema::unsignedByte> (XML::Attribute const& a,
                                  Parser&,
                                  XMLSchema::unsignedByte*)
  {
    wchar_t r;
    std::wstringstream s;
    s << a.value ();
    s >> r;

    return static_cast<unsigned char>(r);
  }

  template<>
  inline
  XMLSchema::unsignedLong
  parse<XMLSchema::unsignedLong> (XML::Element const& e,
                                  Parser& p,
                                  XMLSchema::unsignedLong*)
  {
    unsigned long r;
    std::wstringstream s;
    s << parse<std::wstring> (e, p, 0);
    s >> r;

    return r;
  }

  template<>
  inline
  XMLSchema::unsignedLong
  parse<XMLSchema::unsignedLong> (XML::Attribute const& a,
                                  Parser&,
                                  XMLSchema::unsignedLong*)
  {
    unsigned long r;
    std::wstringstream s;
    s << a.value ();
    s >> r;

    return r;
  }

  template<>
  inline
  XMLSchema::boolean
  parse<XMLSchema::boolean> (XML::Element const& e,
                             Parser& p,
                             XMLSchema::boolean*)
  {
    return parse<std::wstring> (e, p, 0) == L"true";
  }

  template<>
  inline
  XMLSchema::boolean
  parse<XMLSchema::boolean> (XML::Attribute const& a,
                             Parser&,
                             XMLSchema::boolean*)
  {
    return a.value () == L"true";
  }

  // ID
  //
  //
  template<>
  inline
  XMLSchema::ID
  parse<XMLSchema::ID> (std::wstring const& s, Parser&, XMLSchema::ID*)
  {
    return XMLSchema::ID (s);
  }

  template<>
  inline
  XMLSchema::ID
  parse<XMLSchema::ID> (XML::Element const& e, Parser& p, XMLSchema::ID*)
  {
    return parse<XMLSchema::ID> (parse<std::wstring> (e, p, 0), p, 0);
  }

  template<>
  inline
  XMLSchema::ID
  parse<XMLSchema::ID> (XML::Attribute const& a, Parser& p, XMLSchema::ID*)
  {
    return parse<XMLSchema::ID> (a.value (), p, 0);
  }

  // IDREF
  //
  //
  template<>
  inline
  XMLSchema::IDREF
  parse<XMLSchema::IDREF> (std::wstring const& s, Parser&, XMLSchema::IDREF*)
  {
    return XMLSchema::IDREF (s);
  }

  template<>
  inline
  XMLSchema::IDREF
  parse<XMLSchema::IDREF> (XML::Element const& e, Parser& p, XMLSchema::IDREF*)
  {
    return parse<XMLSchema::IDREF> (parse<std::wstring> (e, p, 0), p, 0);
  }

  template<>
  inline
  XMLSchema::IDREF
  parse<XMLSchema::IDREF> (XML::Attribute const& a,
                           Parser& p,
                           XMLSchema::IDREF*)
  {
    return parse<XMLSchema::IDREF> (a.value (), p, 0);
  }


  // string
  //
  //
  template<>
  inline
  XMLSchema::string
  parse<XMLSchema::string> (std::wstring const& s, Parser&, XMLSchema::string*)
  {
    return s;
  }

  template<>
  inline
  XMLSchema::string
  parse<XMLSchema::string> (XML::Element const& e,
                            Parser& p,
                            XMLSchema::string*)
  {
    return parse<XMLSchema::string> (parse<std::wstring> (e, p, 0), p, 0);
  }

  template<>
  inline
  XMLSchema::string
  parse<XMLSchema::string> (XML::Attribute const& a,
                            Parser& p,
                            XMLSchema::string*)
  {
    return parse<XMLSchema::string> (a.value (), p, 0);
  }

  */
}
