// file      : XSCRT/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XSCRT_PARSER_HPP
#define XSCRT_PARSER_HPP

#include <stack>
#include <string>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

//@@ VC6
#if defined (_MSC_VER) && (_MSC_VER < 1300)
namespace std
{
  typedef ::size_t size_t;
}
#endif

namespace XSCRT
{
  namespace XML
  {
    //@@ VC6
    //
    template <typename C>
    std::basic_string<C>
    transcode (XMLCh const* s, C*);

    template <>
    inline
    std::basic_string<char>
    transcode<char> (XMLCh const* s, char*)
    {
      if (s == 0) return std::basic_string<char> ();

      char* buf = xercesc::XMLString::transcode (s);

      std::basic_string<char> r (buf);

      xercesc::XMLString::release (&buf);

      return r;
    }

    template <>
    inline
    std::basic_string<wchar_t>
    transcode<wchar_t> (XMLCh const* s, wchar_t*)
    {
      if (s == 0) return std::basic_string<wchar_t> ();

      // std::wcerr << s << std::endl;

      std::basic_string<wchar_t> r (xercesc::XMLString::stringLen (s), L'0');

      for (std::size_t i (0); *s != XMLCh (0); ++s, ++i)
      {
        r[i] = *s;
      }

      return r;
    }

    template <typename C>
    XMLCh*
    transcode (std::basic_string<C> const& s);

    template <>
    inline
    XMLCh*
    transcode (std::basic_string<char> const& s)
    {
      return xercesc::XMLString::transcode (s.c_str ());
    }

    template <>
    inline
    XMLCh*
    transcode (std::basic_string<wchar_t> const& s)
    {
      //@@ VC6
      std::size_t l = s.length ();

      //@@ VC6
      XMLCh* r = new XMLCh[l + 1];
      XMLCh* ir = r;

      for (std::size_t i (0); i < l; ++ir, ++i)
      {
        *ir = static_cast<XMLCh>(s[i]);
        //std::wcerr << s[i] << "->" << *ir << std::endl;
      }

      *ir = XMLCh (0);

      // std::wcerr << r << std::endl;

      return r;
    }

    template <typename C>
    class Element
    {
      typedef std::basic_string<C> string_;

    public:
      Element (xercesc::DOMElement const* e)
          : e_ (e),
            name_ (transcode<C> (e->getLocalName (), 0)),
            namespace__ (transcode<C> (e->getNamespaceURI (), 0))
      {
      }

      string_
      name () const
      {
        return name_;
      }

      string_
      namespace_ () const
      {
        return namespace__;
      }

    public:
      Element<C>
      parent () const
      {
        return dynamic_cast<xercesc::DOMElement const*>(e_->getParentNode ());
      }

    public:
      string_
      value () const
      {
        return XML::transcode<C> (dom_element ()->getTextContent (), 0);
      }

    public:
      string_
      operator[] (string_ const& s) const
      {
        //@@ VC6
        XMLCh* name = transcode (s);
        XMLCh const* value = e_->getAttribute (name);
        delete[] name;

        return transcode<C> (value, 0);
      }

    public:
      xercesc::DOMElement const*
      dom_element () const
      {
        return e_;
      }

    private:

    private:
      xercesc::DOMElement const* e_;

      string_ name_;
      string_ namespace__;
    };


    template <typename C>
    class Attribute
    {
      typedef std::basic_string<C> string_;

    public:
      Attribute (xercesc::DOMAttr const* a)
          : a_ (a),
            name_ (transcode<C> (a->getLocalName (), 0)),
            value_ (transcode<C> (a->getValue (), 0))
      {
      }

      string_
      name () const
      {
        return name_;
      }

      string_
      value () const
      {
        return value_;
      }

    public:
      xercesc::DOMAttr const*
      dom_attribute () const
      {
        return a_;
      }

    private:

    private:
      xercesc::DOMAttr const* a_;

      string_ name_;
      string_ value_;
    };

    template <typename C>
    std::basic_string<C>
    prefix (std::basic_string<C> const& n)
    {
      std::size_t i (0);
      while (i < n.length () && n[i] != ':') ++i;

      //std::wcerr << "prefix " << n << " "
      //           << std::wstring (n, i == n.length () ? i : 0, i) << std::endl;

      return std::basic_string<C> (n, i == n.length () ? i : 0, i);
    }

    template <typename C>
    std::basic_string<C>
    uq_name (std::basic_string<C> const& n)
    {
      std::size_t i (0);
      while (i < n.length () && n[i] != ':') ++i;

      return std::basic_string<C> (n.c_str () + (i == n.length () ? 0 : i + 1));
    }

    template <typename C>
    std::basic_string<C>
    ns_name (Element<C> const& e, std::basic_string<C> const& n)
    {
      //@@ VC6
      XMLCh* p = 0;

      std::basic_string<C> wp (prefix (n));

      if (!wp.empty ()) p = transcode (wp);

      XMLCh const* xns = e.dom_element ()->lookupNamespaceURI (p);
      std::basic_string<C> ns (
        xns ? transcode<C> (xns, 0) : std::basic_string<C> ());

      delete[] p;
      return ns;

    }

    template <typename C>
    std::basic_string<C>
    fq_name (Element<C> const& e, std::basic_string<C> const& n)
    {
      std::basic_string<C> ns (ns_name (e, n));
      std::basic_string<C> un (uq_name (n));

      return ns.empty () ? un : (ns + C ('#') + un);
    }
  }

  template <typename C>
  class Parser
  {
  public:
    Parser (XML::Element<C> const& e)
        : e_ (e.dom_element ()->getChildNodes ()), ei_ (0),
          a_ (e.dom_element ()->getAttributes ()), ai_ (0)
    {
    }

    bool
    more_elements () const
    {
      return e_->getLength () > ei_;
    }

    XML::Element<C>
    next_element ()
    {
      return XML::Element<C> (
        dynamic_cast<xercesc::DOMElement*> (e_->item (ei_++)));
    }

    bool
    more_attributes () const
    {
      return a_->getLength () > ai_;
    }

    XML::Attribute<C>
    next_attribute ()
    {
      return XML::Attribute<C> (
        dynamic_cast<xercesc::DOMAttr*> (a_->item (ai_++)));
    }

  private:
    xercesc::DOMNodeList const* e_;
    unsigned long ei_;

    xercesc::DOMNamedNodeMap const* a_;
    unsigned long ai_;

  private:
    Parser (Parser const&);

    void
    operator= (Parser const&);
  };
}

#include "XSCRT/Parser.ipp"
#include "XSCRT/Parser.tpp"

#endif  // XSCRT_PARSER_HPP
