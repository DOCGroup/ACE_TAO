#ifndef VALUE_PARSER_HPP
#define VALUE_PARSER_HPP

#include "definitions.hpp"
#include "option_interface.hpp"

#include <xercesc/dom/DOM.hpp>

namespace OCML
{

  class Option_Value_Parser
  {
  public:
    typedef std::map<const Option_Definition*, Option_Interface*>
      interface_map;

    class Unknown_Option_Kind: public std::exception {
    public:
      Unknown_Option_Kind(const std::string& msg)
        : msg_(std::string("ValueParser:Unknown_Option_Kind ") + msg)
      {
      }

      virtual ~Unknown_Option_Kind() throw()
      {
      }

      virtual const char* what() const throw()
      {
        return msg_.c_str();
      }

    private:
      std::string msg_;
    };

    static void parse_xml(Option_Category* root,
                          interface_map& interfaces,
                          xercesc::DOMDocument* doc);

  private:
    Option_Value_Parser(interface_map& interfaces);

    void parse_document(Option_Category* root,
                        xercesc::DOMDocument* doc);

    void parse_category(Option_Category* category,
                        xercesc::DOMNode* node);

    void parse_option(xercesc::DOMNode* node, Option_Interface* interface);

    interface_map& interfaces_;
  };

} // namespace OCML

#endif // VALUE_PARSER_HPP
