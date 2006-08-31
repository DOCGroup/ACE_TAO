// $Id$

#include "OCMLBonX.h"
#include "OCMLUtils.h"
#include <algorithm>

namespace OCML_BON
{

  xercesc::DOMElement*
  Described_ItemImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = doc->createElement(XStr(get_kind_name()));
    node->setAttribute(XStr("name"), XStr(getName()));
    node->setAttribute(XStr("description"), XStr(getDescription()));
    node->setAttribute(XStr("title"), XStr(getTitle()));

    return node;
  }

  xercesc::DOMElement*
  Option_CategoryImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    children_xml_exporter(getOption_Category(), doc, node);
    children_xml_exporter(getOption(), doc, node);

    return node;
  }

  xercesc::DOMElement*
  OptionImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    node->setAttribute(XStr("param-name"), XStr(getParameter_Name()));

    return node;
  }

  xercesc::DOMElement*
  Option_ItemImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    if (isItem_Default_Value())
      node->setAttribute(XStr("default-value"), XStr("true"));
    else
      node->setAttribute(XStr("default-value"), XStr("false"));

    return node;
  }

  xercesc::DOMElement*
  Boolean_OptionImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    if (isBoolean_Default_Value())
      node->setAttribute(XStr("default-value"), XStr("true"));
    else
      node->setAttribute(XStr("default-value"), XStr("false"));

    return node;
  }

  xercesc::DOMElement*
  Integer_OptionImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    node->setAttribute(XStr("default-value"),
                       XStr(getInteger_Default_Value()));

    return node;
  }

  xercesc::DOMElement*
  String_OptionImpl::xml_export(xercesc::DOMDocument* doc)
  {
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);
    
    node->setAttribute(XStr("default-value"),
                       XStr(getString_Default_Value()));

    return node;
  }

  xercesc::DOMElement*
  Enum_OptionImpl::xml_export(xercesc::DOMDocument* doc)
  {
//    xercesc::DOMElement* node = OptionImpl::xml_export(doc);
//    children_xml_exporter(getEnum_Item(), doc, node);
    xercesc::DOMElement* node = Described_ItemImpl::xml_export(doc);

    typedef std::set<OCML_BON::Enum_Item> children_t;
    children_t children = getEnum_Item();
    
    for (children_t::iterator iter = children.begin(); iter != children.end();
         ++iter)
    {
      if ((*iter)->isItem_Default_Value())
      {
        node->setAttribute(XStr("default-value"),
                           XStr((*iter)->getTitle()));
      }
    }
    return node;
  }

} // namespace OCML_BON
