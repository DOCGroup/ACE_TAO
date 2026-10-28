// file      : XSCRT/XML.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#ifndef XSCRT_XML_HPP
#define XSCRT_XML_HPP

#include <string>
#include <algorithm>
#include <type_traits>
#include <utility>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

namespace XSCRT
{
  namespace XML
  {
    template <typename C>
    std::basic_string<C>
    transcode (XMLCh const* s, C*);

    template <typename C>
    XMLCh*
    transcode (std::basic_string<C> const& s);

    //
    //
    //
    class string
    {
    public :
      template <typename C>
      string (std::basic_string<C> const& s)
          : s_ (XSCRT::XML::transcode<C> (s)),
            xerces_allocated_ (std::is_same<C, char>::value)
      {
      }

      template <typename C>
      string (C const* s)
          : s_ (XSCRT::XML::transcode<C> (s)),
            xerces_allocated_ (std::is_same<C, char>::value)
      {
      }

      string (string const& s)
          : s_ (copy (s.s_)), xerces_allocated_ (false)
      {
      }

      string (string&& s) noexcept
          : s_ (s.s_), xerces_allocated_ (s.xerces_allocated_)
      {
        s.s_ = nullptr;
      }

      string& operator= (string const& s)
      {
        if (this != &s)
        {
          XMLCh* p (copy (s.s_));
          release ();
          s_ = p;
          xerces_allocated_ = false;
        }
        return *this;
      }

      string& operator= (string&& s) noexcept
      {
        if (this != &s)
        {
          release ();
          s_ = s.s_;
          xerces_allocated_ = s.xerces_allocated_;
          s.s_ = nullptr;
        }
        return *this;
      }

      ~string ()
      {
        release ();
      }

      XMLCh const*
      c_str () const
      {
        return s_;
      }

    private :
      static XMLCh* copy (XMLCh const* s)
      {
        if (s == nullptr) return nullptr;
        std::size_t const n (xercesc::XMLString::stringLen (s));
        XMLCh* const r (new XMLCh[n + 1]);
        std::copy (s, s + n + 1, r);
        return r;
      }

      void release () noexcept
      {
        if (xerces_allocated_)
          xercesc::XMLString::release (&s_);
        else
          delete[] s_;
        s_ = nullptr;
      }

      XMLCh* s_;
      bool xerces_allocated_;
    };


    template <>
    inline
    std::basic_string<char>
    transcode<char> (XMLCh const* s, char*)
    {
      if (s == nullptr) return std::basic_string<char> ();

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
      if (s == nullptr) return std::basic_string<wchar_t> ();

      // std::wcerr << s << std::endl;

      std::basic_string<wchar_t> r (xercesc::XMLString::stringLen (s), L'0');

      for (std::size_t i (0); *s != XMLCh (0); ++s, ++i)
      {
        r[i] = *s;
      }

      return r;
    }

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
      std::size_t const length (s.length ());
      XMLCh* const result (new XMLCh[length + 1]);
      std::transform (s.begin (), s.end (), result,
                      [] (wchar_t c) { return static_cast<XMLCh> (c); });
      result[length] = XMLCh {};

      return result;
    }


    template <typename C>
    class Element;

    template <typename C>
    std::basic_string<C>
    ns_prefix (std::basic_string<C> const& ns, Element<C> const& e);

    // Casting helpers, made necessary by the Xerces project's braindead
    // avoidance of RTTI.
    template <typename DERIVED> struct dom_traits;

    // Specializations for different node types
    template <>
    struct dom_traits<xercesc::DOMElement *>
    {
      static const xercesc::DOMNode::NodeType node_type = xercesc::DOMNode::ELEMENT_NODE;
    };

    template <>
    struct dom_traits<xercesc::DOMAttr *>
    {
      static const xercesc::DOMNode::NodeType node_type = xercesc::DOMNode::ATTRIBUTE_NODE;
    };

    template <typename DERIVED_PTR>
    DERIVED_PTR dom_cast (xercesc::DOMNode *node)
    {
      DERIVED_PTR elem = nullptr;
      if ((node != nullptr) &&
          (node->getNodeType () == dom_traits<DERIVED_PTR>::node_type))
        {
          elem = reinterpret_cast <DERIVED_PTR> (node);
        }
      return elem;
    }

    template <typename C>
    class Element
    {
      typedef std::basic_string<C> string_;

    public:
      Element (xercesc::DOMElement const* e)
          : e_ (nullptr),
            ce_ (e),
            name_ (transcode<C> (e->getLocalName (), nullptr)),
            namespace__ (transcode<C> (e->getNamespaceURI (), nullptr))
      {
      }

      Element (xercesc::DOMElement* e)
          : e_ (e),
            ce_ (e),
            name_ (transcode<C> (e->getLocalName (), nullptr)),
            namespace__ (transcode<C> (e->getNamespaceURI (), nullptr))
      {
      }

      Element (string_ const& name, Element& parent)
          : e_ (nullptr),
            ce_ (nullptr),
            name_ (name)
      {
        xercesc::DOMDocument* doc (
          parent.dom_element ()->getOwnerDocument ());

        e_ = doc->createElement (string (name).c_str ());

        parent.dom_element ()->appendChild (e_);

        ce_ = e_;
      }

      Element (string_ const& name, string_ const& ns, Element& parent)
          : e_ (nullptr),
            ce_ (nullptr),
            name_ (name),
            namespace__ (ns)
      {
        string_ prefix (ns_prefix (ns, parent));

        xercesc::DOMDocument* doc (
          parent.dom_element ()->getOwnerDocument ());

        e_ = doc->createElementNS (
          string (ns).c_str (),
          string (prefix.empty ()
                ? name
                : prefix + string_ (1, ':') + name).c_str ());

        parent.dom_element ()->appendChild (e_);

        ce_ = e_;
      }

    public:
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
        return dom_cast <xercesc::DOMElement const*>(ce_->getParentNode ());
      }

    public:
      string_
      value () const
      {
        return XML::transcode<C> (dom_element ()->getTextContent (), nullptr);
      }

      void
      value (string_ const& v)
      {
        xercesc::DOMText* text (
          dom_element ()->getOwnerDocument ()->createTextNode(
            string (v).c_str ()));

        dom_element ()->appendChild (text);
      }

    public:
      string_
      operator[] (string_ const& s) const
      {
        XMLCh const* value = ce_->getAttribute (string (s).c_str ());

        return transcode<C> (value, nullptr);
      }

    public:
      xercesc::DOMElement const*
      dom_element () const
      {
        return ce_;
      }

      xercesc::DOMElement*
      dom_element ()
      {
        return e_;
      }

    private:
      xercesc::DOMElement* e_;
      xercesc::DOMElement const* ce_;

      string_ name_;
      string_ namespace__;
    };


    template <typename C>
    class Attribute
    {
      typedef std::basic_string<C> string_;

    public:
      Attribute (xercesc::DOMAttr const* a)
          : a_ (nullptr),
            ca_ (a),
            name_ (transcode<C> (a->getLocalName (), nullptr)),
            value_ (transcode<C> (a->getValue (), nullptr))
      {
      }

      Attribute (xercesc::DOMAttr* a)
          : a_ (a),
            ca_ (a),
            name_ (transcode<C> (a->getLocalName (), nullptr)),
            value_ (transcode<C> (a->getValue (), nullptr))
      {
      }

      Attribute (string_ const& name,
                 string_ const& v,
                 Element<C>& parent)
          : a_ (nullptr),
            ca_ (nullptr),
            name_ (name),
            value_ ()
      {
        xercesc::DOMDocument* doc (
          parent.dom_element ()->getOwnerDocument ());

        a_ = doc->createAttribute (string (name).c_str ());

        value (v);

        parent.dom_element ()->setAttributeNode (a_);

        ca_ = a_;
      }

      Attribute (string_ const& name,
                 string_ const& ns,
                 string_ const& v,
                 Element<C>& parent)
          : a_ (nullptr),
            ca_ (nullptr),
            name_ (name),
            value_ ()
      {
        string_ prefix (ns_prefix (ns, parent));

        xercesc::DOMDocument* doc (
          parent.dom_element ()->getOwnerDocument ());

        a_ = doc->createAttributeNS (
          string (ns).c_str (),
          string (prefix.empty ()
                ? name
                : prefix + string_ (1, ':') + name).c_str ());

        value (v);

        parent.dom_element ()->setAttributeNodeNS (a_);

        ca_ = a_;
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

      void
      value (string_ const& v)
      {
        value_ = v;
        a_->setValue (string (v).c_str ());
      }

    public:
      xercesc::DOMAttr const*
      dom_attribute () const
      {
        return ca_;
      }

      xercesc::DOMAttr*
      dom_attribute ()
      {
        return a_;
      }

    private:

    private:
      xercesc::DOMAttr* a_;
      xercesc::DOMAttr const* ca_;

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
      std::basic_string<C> wp (prefix (n));

      XMLCh const* xns = e.dom_element ()->lookupNamespaceURI (
        wp.empty () ? nullptr : string (wp).c_str ());


      std::basic_string<C> ns (
        xns ? transcode<C> (xns, nullptr) : std::basic_string<C> ());

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

    class no_prefix {};

    template <typename C>
    std::basic_string<C>
    ns_prefix (std::basic_string<C> const& ns, Element<C> const& e)
    {
      string xns (ns);

      XMLCh const* p (e.dom_element ()->lookupPrefix (xns.c_str ()));

      if (p == nullptr)
      {
        bool const r (e.dom_element ()->isDefaultNamespace (xns.c_str ()));

        if (r)
        {
          return std::basic_string<C> ();
        }
        else
        {
          throw no_prefix ();
        }
      }

      return transcode<C> (p, nullptr);
    }
  }
}

#endif  // XSCRT_XML_HPP
