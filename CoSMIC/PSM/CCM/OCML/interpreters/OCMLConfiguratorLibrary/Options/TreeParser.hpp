// $Id$

#ifndef TREE_PARSER_HPP
#define TREE_PARSER_HPP

#include "Options.hpp"

#include <xercesc/dom/DOM.hpp>

namespace OCML
{

  class OptionTreeParser
  {
  public:
    class UnknownOptionKind: public std::exception {
    public:
      UnknownOptionKind(const std::string& msg);
    
      virtual ~UnknownOptionKind() throw();
    
      virtual const char* what() const throw();
    
    private:
      std::string msg_;
    };

    static OptionCategory* parse_xml(xercesc::DOMDocument* doc);

  private:
    OptionTreeParser();
    OptionCategory* parse_document(xercesc::DOMDocument* doc);

    void parse_attributes(xercesc::DOMNode* node,
                          XMLUnicodeString& name,
                          XMLUnicodeString& description);

    void parse_attributes(xercesc::DOMNode* node,
                          XMLUnicodeString& name,
                          XMLUnicodeString& description,
                          XMLUnicodeString& default_value);

    OptionCategory* parse_category(xercesc::DOMNode* node);
    BooleanOption* parse_boolean_option(xercesc::DOMNode* node);
    IntegerOption* parse_integer_option(xercesc::DOMNode* node);
    StringOption* parse_string_option(xercesc::DOMNode* node);
    EnumOption* parse_enum_option(xercesc::DOMNode* node);

    /// first: the name of the item, second: is true if it is default item.
    std::pair<std::string, bool> parse_enum_item(xercesc::DOMNode* node);
  };

} // namespace OCML

#include "TreeParser.ipp"

#endif // TREE_PARSER_HPP
