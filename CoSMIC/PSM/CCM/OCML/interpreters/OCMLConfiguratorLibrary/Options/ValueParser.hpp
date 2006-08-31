#ifndef VALUE_PARSER_HPP
#define VALUE_PARSER_HPP

#include "Options.hpp"

#include <xercesc/dom/DOM.hpp>

namespace OCML
{

  class OptionValueParser
  {
  public:
    class UnknownOptionKind: public std::exception {
    public:
      UnknownOptionKind(const std::string& msg)
        : msg_(std::string("ValueParser:UnknownOptionKind ") + msg)
      {
      }

      virtual ~UnknownOptionKind() throw()
      {
      }

      virtual const char* what() const throw()
      {
        return msg_.c_str();
      }

    private:
      std::string msg_;
    };

    static void parse_xml(OptionCategory* root,
                          xercesc::DOMDocument* doc);

  private:
    OptionValueParser();
    void parse_document(OptionCategory* root,
                        xercesc::DOMDocument* doc);

    void parse_category(OptionCategory* category,
                        xercesc::DOMNode* node);
    void parse_boolean_option(BooleanOption* option,
                              xercesc::DOMNode* node);
    void parse_integer_option(IntegerOption* option,
                              xercesc::DOMNode* node);
    void parse_string_option(StringOption* option,
                             xercesc::DOMNode* node);
    void parse_enum_option(EnumOption* option,
                           xercesc::DOMNode* node);
  };

} // namespace OCML

#endif // VALUE_PARSER_HPP
