// $Id$

#include <sstream>

#include "tree_parser.hpp"
#include <common/string_utils.hpp>

using namespace xercesc;
using namespace OCML;

/**
 * Static public factory method. Parses an xml option tree and returns the
 * generated root category.
 */

Option_Category*
Option_Tree_Parser::parse_xml(DOMDocument* doc)
{
  Option_Tree_Parser parser;
  if (doc)
  {
    Option_Category* result = parser.parse_document(doc);
    return result;
  }
  return 0;
}

Option_Tree_Parser::Option_Tree_Parser()
{
}

Option_Category*
Option_Tree_Parser::parse_document(DOMDocument* doc)
{
  Option_Category* result = parse_category(doc->getDocumentElement());
  return result;
}

void
Option_Tree_Parser::parse_attributes(DOMNode* node,
                                     XMLUnicodeString& name,
                                     XMLUnicodeString& description)
{
  XMLUnicodeString name_tag("name");
  XMLUnicodeString descr_tag("description");

  DOMNamedNodeMap* attributes = node->getAttributes();

  name =
    attributes->getNamedItem(name_tag)->getNodeValue();
  description =
    attributes->getNamedItem(descr_tag)->getNodeValue();
}

void
Option_Tree_Parser::parse_attributes(DOMNode* node,
                                     XMLUnicodeString& name,
                                     XMLUnicodeString& description,
                                     XMLUnicodeString& default_value)
{
  XMLUnicodeString name_tag("name");
  XMLUnicodeString descr_tag("description");
  XMLUnicodeString default_tag("default-value");

  DOMNamedNodeMap* attributes = node->getAttributes();
  DOMNode* attr_node;

  name = 
    attributes->getNamedItem(name_tag)->getNodeValue();
  description =
    attributes->getNamedItem(descr_tag)->getNodeValue();
  attr_node =
    attributes->getNamedItem(default_tag);
  if (attr_node)
    default_value = attr_node->getNodeValue();
}

Option_Category*
Option_Tree_Parser::parse_category(DOMNode* node)
{
  XMLUnicodeString name, description;
  parse_attributes(node, name, description);

  Option_Category* result =
    new Option_Category(name.str().c_str(), description.str().c_str());

  DOMNodeList* children = node->getChildNodes();

  for (size_t i = 0; i < children->getLength(); ++i)
  {
    DOMNode* child_node = children->item(i);
    XMLUnicodeString child_node_name = child_node->getNodeName();

    if (child_node_name	== "OptionCategory")
      result->add_category(parse_category(child_node));
    else if (child_node_name == "BooleanOption")
      result->add_option(parse_boolean_option(child_node));
    else if (child_node_name == "StringOption")
      result->add_option(parse_string_option(child_node));
    else if (child_node_name == "IntegerOption")
      result->add_option(parse_integer_option(child_node));
//    else if (child_node_name == "EnumOption")
//      result->add_option(parse_enum_option(child_node));
    //    else
    //      throw UnknownOptionKind(child_node_name.str());
  }

  return result;
}

Boolean_Option_Definition*
Option_Tree_Parser::parse_boolean_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  bool val = default_value.str() == "true";

  return
    new Boolean_Option_Definition(name.str().c_str(), 
                                  description.str().c_str(),
                                  val);
}

Integer_Option_Definition*
Option_Tree_Parser::parse_integer_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  int int_default_value = 0;
  if (default_value.str() != "")
    {
      std::stringstream ss(default_value.str().c_str());
      ss >> int_default_value;
    }

  return
    new Integer_Option_Definition(name.str().c_str(),
                                  description.str().c_str(),
                                  int_default_value);
}

String_Option_Definition*
Option_Tree_Parser::parse_string_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  return
    new String_Option_Definition(name.str().c_str(),
                                 description.str().c_str(),
                                 default_value.str().c_str());
}
/*
EnumOption*
Option_Tree_Parser::parse_enum_option(DOMNode* node)
{
  XMLUnicodeString name, description;
  parse_attributes(node, name, description);

  EnumOption* result =
    new EnumOption(name.str().c_str(), description.str().c_str());

  DOMNodeList* children = node->getChildNodes();

  for (size_t i = 0; i < children->getLength(); ++i)
    {
      XMLUnicodeString item_node_name = children->item(i)->getNodeName();
      if (item_node_name.str() == std::string("Item"))
        {
          std::pair<std::string, bool> item =
            parse_enum_item(children->item(i));
          
          result->add_item(item.first.c_str(), item.second);
        }
    }
  return result;
}

std::pair<std::string, bool>
Option_Tree_Parser::parse_enum_item(DOMNode* node)
{
  std::pair<std::string, bool> result;
  XMLUnicodeString name_tag("name");
  XMLUnicodeString default_tag("default-value");

  DOMNamedNodeMap* attributes = node->getAttributes();
  XMLUnicodeString name =
    attributes->getNamedItem(name_tag)->getNodeValue();
  XMLUnicodeString default_value =
    attributes->getNamedItem(default_tag)->getNodeValue();
  
  result.first = name.str();
  result.second = (default_value.str() == "true");

  return result;
}
*/
