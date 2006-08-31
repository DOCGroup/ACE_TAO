#include "SvcConfExporter.hpp"

#include "../Common/XercesHelpers.hpp"
#include "../Common/StringUtils.hpp"

#include <string.h>

std::string
SvcConfExporter::parse_option(xercesc::DOMNode* node, xercesc::DOMDocument* doc)
{
  xercesc::DOMNamedNodeMap* attributes = node->getAttributes();
  XMLUnicodeString name =
    attributes->getNamedItem(XMLUnicodeString("name"))->getNodeValue();

  std::string node_value = "";

  xercesc::DOMNodeList* children = node->getChildNodes();
  for (size_t i = 0; i < children->getLength(); ++i)
    {
      if (children->item(i)->getNodeType() == xercesc::DOMNode::TEXT_NODE)
	{
	  XMLUnicodeString value = children->item(i)->getNodeValue();
	  node_value = value.str();
	}
    }

  node_value = delete_begin_end_spaces(node_value);

  return std::string("-") + name.str() + " " + node_value;
}

void
SvcConfExporter::parse_category(xercesc::DOMNode* source,
                                xercesc::DOMDocument* target,
                                xercesc::DOMNode* parent)
{
  std::string params;
  xercesc::DOMNodeList* children = source->getChildNodes();

  for (size_t i = 0; i < children->getLength(); ++i)
    {
      xercesc::DOMNode* child_node = children->item(i);
      XMLUnicodeString child_node_name = child_node->getNodeName();

      if (child_node_name == "OptionCategory")
          parse_category(child_node, target, parent);
      else if (child_node_name == "BooleanOption")
          params += parse_option(child_node, target);
      else if (child_node_name == "StringOption")
          params += parse_option(child_node, target);
      else if (child_node_name == "IntegerOption")
          params += parse_option(child_node, target);
      else if (child_node_name == "EnumOption")
          params += parse_option(child_node, target);
    }

  if (params.size())
    {
      xercesc::DOMNamedNodeMap* attributes = source->getAttributes();
      XMLUnicodeString name =
        attributes->getNamedItem(XMLUnicodeString("name"))->getNodeValue();
      
      XercesAutoPtr<xercesc::DOMElement> result =
        target->createElement(XMLUnicodeString("static"));
      result->setAttribute(XMLUnicodeString("id"), name);
      result->setAttribute(XMLUnicodeString("params"),
                           XMLUnicodeString(params.c_str()));

      parent->appendChild(result.release());
    }
}

char*
SvcConfExporter::generate_svc_conf(const char* values)
{
  if (!values)
    return NULL;

  size_t size = strlen(values);
  XercesSystem system;
  system.initialize();
  XercesAutoPtr<xercesc::DOMBuilder> parser = system.new_parser();
  xercesc::DOMDocument *value_doc =
    system.parse_string(parser.get(), "OCMLValues",
                        values, size);

  XercesAutoPtr<xercesc::DOMDocument> new_doc =
    system.impl()->createDocument(0, XMLUnicodeString("test"), 0);
  XercesAutoPtr<xercesc::DOMElement> node =
    new_doc->createElement(XMLUnicodeString("ACE_Svc_Conf"));

  parse_category(value_doc->getDocumentElement(), new_doc.get(), node.get());

  XercesAutoPtr<xercesc::DOMWriter> writer =
    system.impl()->createDOMWriter();

  XMLUnicodeString result;
  if (new_doc->getDocumentElement())
    result = writer->writeToString(*(node.get()));

  return strdup(result.str().c_str());
}
