// $Id$

#include <sstream>
#include "TreeParser.hpp"

#include "../../Common/StringUtils.hpp"

using namespace xercesc;
using namespace OCML;

/**
 * Static public factory method. Parses an xml option tree and returns the
 * generated root category.
 */

OptionCategory*
OptionTreeParser::parse_xml(DOMDocument* doc)
{
  OptionTreeParser parser;
  if (doc)
    return parser.parse_document(doc);
  return 0;
}

OptionTreeParser::OptionTreeParser()
{
}

OptionCategory*
OptionTreeParser::parse_document(DOMDocument* doc)
{
  return parse_category(doc->getDocumentElement());
}

void
OptionTreeParser::parse_attributes(DOMNode* node,
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
OptionTreeParser::parse_attributes(DOMNode* node,
                                   XMLUnicodeString& name,
                                   XMLUnicodeString& description,
                                   XMLUnicodeString& default_value)
{
  XMLUnicodeString name_tag("name");
  XMLUnicodeString descr_tag("description");
  XMLUnicodeString default_tag("default-value");

  DOMNamedNodeMap* attributes = node->getAttributes();

  name =
    attributes->getNamedItem(name_tag)->getNodeValue();
  description =
    attributes->getNamedItem(descr_tag)->getNodeValue();
  default_value =
    attributes->getNamedItem(default_tag)->getNodeValue();
}

OptionCategory*
OptionTreeParser::parse_category(DOMNode* node)
{
  XMLUnicodeString name, description;
  parse_attributes(node, name, description);

  OptionCategory* result =
    new OptionCategory(name.str().c_str(), description.str().c_str());

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
    else if (child_node_name == "EnumOption")
      result->add_option(parse_enum_option(child_node));
    //    else
    //      throw UnknownOptionKind(child_node_name.str());
  }

  return result;
}

BooleanOption*
OptionTreeParser::parse_boolean_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  bool val = default_value.str() == "true";

  return new BooleanOption(name.str().c_str(), description.str().c_str(),
                           val);
}

IntegerOption*
OptionTreeParser::parse_integer_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  int int_default_value = 0;
  if (default_value.str() != "")
    {
      std::stringstream ss(default_value.str().c_str());
      ss >> int_default_value;
    }

  return new IntegerOption(name.str().c_str(), description.str().c_str(),
                           int_default_value);
}

StringOption*
OptionTreeParser::parse_string_option(DOMNode* node)
{
  XMLUnicodeString name, description, default_value;
  parse_attributes(node, name, description, default_value);

  return new StringOption(name.str().c_str(), description.str().c_str(),
                          default_value.str().c_str());
}

EnumOption*
OptionTreeParser::parse_enum_option(DOMNode* node)
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
OptionTreeParser::parse_enum_item(DOMNode* node)
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
