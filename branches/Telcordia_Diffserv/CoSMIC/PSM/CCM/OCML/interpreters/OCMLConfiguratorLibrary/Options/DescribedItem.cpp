#include "DescribedItem.hpp"

using namespace OCML;

/**
 *  Destructor - nop
 */
DescribedItem::~DescribedItem()
{
}

/**
 * This method inserts the name attribute and sets the value for it.
 */
xercesc::DOMElement*
DescribedItem::write(xercesc::DOMDocument* doc) const
{
  xercesc::DOMElement* element =
    doc->createElement(XMLUnicodeString(type_string()));

  element->setAttribute(XMLUnicodeString("name"),
			XMLUnicodeString(name().c_str()));
  return element;
}
