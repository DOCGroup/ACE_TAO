#include <sstream>
#include "ValueParser.hpp"
#include "../../Common/StringUtils.hpp"

using namespace xercesc;
using namespace OCML;

/**
 * Static public factory method. Parses an xml option value tree and sets the
 * values for the option.
 */
void
OptionValueParser::parse_xml(OptionCategory* root, DOMDocument* doc)
{
  OptionValueParser parser;
  parser.parse_document(root, doc);
}

OptionValueParser::OptionValueParser()
{
}

void
OptionValueParser::parse_document(OptionCategory* root, DOMDocument* doc)
{
  DOMNode *document_element = doc->getDocumentElement();
  // If the given xml file does not contain the specified values
  // leave the values as un-initialized.
  // This allows the applications to send empty-strings as value document
  if (document_element)
    parse_category(root, document_element);
}

void
OptionValueParser::parse_category(OptionCategory* category, DOMNode* node)
{
  DOMNodeList* children = node->getChildNodes();

  // Traverse the children
  for (size_t i = 0; i < children->getLength(); ++i)
  {
    DOMNode* item = children->item(i);
    
    if (item->getNodeType() != DOMNode::ELEMENT_NODE)
        continue;
    
    XMLUnicodeString node_name =
      item->getNodeName();

    DOMNamedNodeMap* attributes = item->getAttributes();
    
    XMLUnicodeString name =
      attributes->getNamedItem(XMLUnicodeString("name"))->getNodeValue();

    if (node_name.str() == std::string("OptionCategory"))
      {
        // Parsing a category node
        parse_category(category->category(name.str().c_str()), item);
      }
    else
      {
        // Parsing an option node
        Option* option = category->option(name.str().c_str());

        if (option && (node_name.str() == std::string("BooleanOption")))
          {
            parse_boolean_option((BooleanOption*) option, item);
          }
        else if (option && (node_name.str() == std::string("StringOption")))
          {
            parse_string_option((StringOption*) option, item);
          }
        else if (option && (node_name.str() == std::string("IntegerOption")))
          {
            parse_integer_option((IntegerOption*) option, item);
          }
        else if (option && (node_name.str() == std::string("EnumOption")))
          {
            parse_enum_option((EnumOption*) option, item);
          }
/*
        else
          throw UnknownOptionKind(name.str());
*/
      }
  }
}

void
OptionValueParser::parse_boolean_option(BooleanOption* option, DOMNode* node)
{
  std::string node_value = "";

  DOMNodeList* children = node->getChildNodes();
  for (size_t i = 0; i < children->getLength(); ++i)
    {
      if (children->item(i)->getNodeType() == DOMNode::TEXT_NODE)
  {
    XMLUnicodeString value = children->item(i)->getNodeValue();
    node_value = value.str();
  }
    }

  node_value = delete_begin_end_spaces(node_value);
  if (node_value == std::string("true"))
    {
      option->set(true);
    }
  else
    {
      option->set(false);
    }
}

void
OptionValueParser::parse_integer_option(IntegerOption* option, DOMNode* node)
{
  std::string node_value = "";

  DOMNodeList* children = node->getChildNodes();
  for (size_t i = 0; i < children->getLength(); ++i)
    {
      if (children->item(i)->getNodeType() == DOMNode::TEXT_NODE)
  {
    XMLUnicodeString value = children->item(i)->getNodeValue();
    node_value = value.str();
  }
    }

  node_value = delete_begin_end_spaces(node_value);
  std::istringstream os(node_value.c_str());
  int int_value;
  os >> int_value;
  option->set(int_value);
}

void
OptionValueParser::parse_string_option(StringOption* option, DOMNode* node)
{
  std::string node_value = "";

  DOMNodeList* children = node->getChildNodes();
  for (size_t i = 0; i < children->getLength(); ++i)
    {
      if (children->item(i)->getNodeType() == DOMNode::TEXT_NODE)
  {
    XMLUnicodeString value = children->item(i)->getNodeValue();
    node_value = value.str();
  }
    }

  node_value = delete_begin_end_spaces(node_value);
  option->set(node_value.c_str());
}

void
OptionValueParser::parse_enum_option(EnumOption* option, DOMNode* node)
{
  std::string node_value = "";
  
  DOMNodeList* children = node->getChildNodes();
  for (size_t i = 0; i < children->getLength(); ++i)
    {
      if (children->item(i)->getNodeType() == DOMNode::TEXT_NODE)
        {
          XMLUnicodeString value = children->item(i)->getNodeValue();
          node_value = value.str();
        }
    }

  node_value = delete_begin_end_spaces(node_value);
  option->set(node_value.c_str());
}
