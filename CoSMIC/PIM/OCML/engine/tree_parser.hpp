// $Id$

#ifndef _TREE_PARSER_HPP_
#define _TREE_PARSER_HPP_

#include <common/string_utils.hpp>
#include "definitions.hpp"
#include "definitions.hpp"

#include <xercesc/dom/DOM.hpp>

namespace OCML
{

  class Option_Tree_Parser
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

    static Option_Category* parse_xml(xercesc::DOMDocument* doc);

  private:
    Option_Tree_Parser();
    Option_Category* parse_document(xercesc::DOMDocument* doc);

    void parse_attributes(xercesc::DOMNode* node,
                          XMLUnicodeString& name,
                          XMLUnicodeString& description);

    void parse_attributes(xercesc::DOMNode* node,
                          XMLUnicodeString& name,
                          XMLUnicodeString& description,
                          XMLUnicodeString& default_value);

    Option_Category* parse_category(xercesc::DOMNode* node);
    Boolean_Option_Definition* parse_boolean_option(xercesc::DOMNode* node);
    Integer_Option_Definition* parse_integer_option(xercesc::DOMNode* node);
    String_Option_Definition* parse_string_option(xercesc::DOMNode* node);
//    EnumOption* parse_enum_option(xercesc::DOMNode* node);

    /// first: the name of the item, second: is true if it is default item.
//    std::pair<std::string, bool> parse_enum_item(xercesc::DOMNode* node);
  };

} // namespace OCML

#include "tree_parser.ipp"

#endif // _TREE_PARSER_HPP_
