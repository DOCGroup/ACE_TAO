#include "Option.hpp"

using namespace OCML;

/**
 * If the option is assigned returns the created node by the
 * DescribedItem::write, otherwise NULL.
 *
 * @see DescribedItem::write.
 */
xercesc::DOMElement*
Option::write(xercesc::DOMDocument* doc) const
{
  if (assigned())
    return DescribedItem::write(doc);
  else
    return NULL;
}
