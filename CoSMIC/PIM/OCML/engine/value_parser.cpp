#include <sstream>
#include "value_parser.hpp"
#include <common/string_utils.hpp>

using namespace xercesc;
using namespace OCML;

/**
 * Static public factory method. Parses an xml option value tree and sets the
 * values for the option.
 */
void
Option_Value_Parser::parse_xml(Option_Category* root, interface_map& interfaces,
                             DOMDocument* doc)
{
  Option_Value_Parser parser(interfaces);
  parser.parse_document(root, doc);
}

Option_Value_Parser::Option_Value_Parser(interface_map& interfaces)
  : interfaces_(interfaces)
{
}

void
Option_Value_Parser::parse_document(Option_Category* root, DOMDocument* doc)
{
  DOMNode *document_element = doc->getDocumentElement();
  // If the given xml file does not contain the specified values
  // leave the values as un-initialized.
  // This allows the applications to send empty-strings as value document
  if (document_element)
    parse_category(root, document_element);
}

void
Option_Value_Parser::parse_category(Option_Category* category, DOMNode* node)
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
        Option_Definition* option = category->option(name.str().c_str());
        Option_Interface* interface = interfaces_[option];
        parse_option(item, interface);
      }
  }
}

void
Option_Value_Parser::parse_option(DOMNode* node, Option_Interface* interface)
{
  // Fetch the value
  std::string node_value = "";

  DOMNodeList* opt_children = node->getChildNodes();
  for (size_t i = 0; i < opt_children->getLength(); ++i)
  {
    if (opt_children->item(i)->getNodeType() == DOMNode::TEXT_NODE)
    {
      XMLUnicodeString value = opt_children->item(i)->getNodeValue();
      node_value = value.str();
    }
  }

  node_value = delete_begin_end_spaces(node_value);

  // Set the value
  interface->str_value(node_value.c_str());
}

