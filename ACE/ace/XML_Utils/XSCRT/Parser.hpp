// file      : XSCRT/Parser.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef XSCRT_PARSER_HPP
#define XSCRT_PARSER_HPP

#include <stdexcept>
#include <string>

#include "ace/XML_Utils/XSCRT/XML.hpp"

namespace XSCRT
{
  template <typename C>
  class Parser
  {
  public:
    Parser (XML::Element<C> const &e)
      : e_ (e.dom_element ()->getChildNodes ())
      , a_ (e.dom_element ()->getAttributes ())
    {
    }

    bool more_elements () const
    {
      for (XMLSize_t i (ei_); i < e_->getLength (); ++i)
        if (e_->item (i)->getNodeType () == xercesc::DOMNode::ELEMENT_NODE)
          return true;
      return false;
    }

    XML::Element<C> next_element ()
    {
      while (ei_ < e_->getLength ())
      {
        xercesc::DOMNode *const n (e_->item (ei_++));
        if (n->getNodeType () == xercesc::DOMNode::ELEMENT_NODE)
          return XML::Element<C> (XML::dom_cast<xercesc::DOMElement *> (n));
      }
      throw std::out_of_range ("No more child elements");
    }

    bool more_attributes () const
    {
      return a_->getLength () > ai_;
    }

    XML::Attribute<C> next_attribute ()
    {
      return XML::Attribute<C> (XML::dom_cast<xercesc::DOMAttr *> (a_->item (ai_++)));
    }

  private:
    xercesc::DOMNodeList const *e_;
    XMLSize_t ei_{};

    xercesc::DOMNamedNodeMap const *a_;
    XMLSize_t ai_{};

  private:
    Parser (Parser const &);

    void operator= (Parser const &);
  };
}

#endif // XSCRT_PARSER_HPP
