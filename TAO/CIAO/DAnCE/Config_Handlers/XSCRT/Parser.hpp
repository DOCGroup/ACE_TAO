// file      : XSCRT/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef XSCRT_PARSER_HPP
#define XSCRT_PARSER_HPP

#include <string>

#include <XSCRT/XML.hpp>

namespace XSCRT
{
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

#include <XSCRT/Parser.ipp>
#include <XSCRT/Parser.tpp>

#endif  // XSCRT_PARSER_HPP
