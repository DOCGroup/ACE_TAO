#include "ACEXML/common/AttributesImpl.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_main.h"
#include <utility>

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACEXML_Attribute original (ACE_TEXT ("urn:move"), ACE_TEXT ("name"),
                             ACE_TEXT ("p:name"), ACE_TEXT ("CDATA"),
                             ACE_TEXT ("value"));
  ACEXML_Char const * const original_value = original.value ();
  ACEXML_Attribute moved (std::move (original));
  ACEXML_Attribute assigned;
  assigned = std::move (moved);
  if (assigned.value () == nullptr || assigned.value () != original_value ||
      original.value () != nullptr ||
      moved.value () != nullptr ||
      ACE_OS::strcmp (assigned.value (), ACE_TEXT ("value")) != 0)
    {
      return 1;
    }

  ACEXML_AttributesImpl attributes (1);
  if (attributes.addAttribute (ACE_TEXT ("urn:one"), ACE_TEXT ("one"),
                               ACE_TEXT ("p:one"), ACE_TEXT ("CDATA"),
                               ACE_TEXT ("first")) != 0 ||
      attributes.addAttribute (ACE_TEXT ("urn:two"), ACE_TEXT ("two"),
                               ACE_TEXT ("p:two"), ACE_TEXT ("CDATA"),
                               ACE_TEXT ("second")) != 1 ||
      attributes.addAttribute (ACE_TEXT ("urn:one"), ACE_TEXT ("one"),
                               ACE_TEXT ("p:one"), ACE_TEXT ("CDATA"),
                               ACE_TEXT ("duplicate")) != -1)
    {
      return 1;
    }

  ACEXML_AttributesImpl copy (attributes);
  if (copy.getLength () != 2 ||
      ACE_OS::strcmp (copy.getValue (ACE_TEXT ("p:two")),
                      ACE_TEXT ("second")) != 0)
    {
      return 1;
    }

  if (attributes.removeAttribute (0) != 0 ||
      attributes.getLength () != 1 ||
      ACE_OS::strcmp (attributes.getValue (size_t {}),
                      ACE_TEXT ("second")) != 0 ||
      attributes.removeAttribute (1) != -1)
    {
      return 1;
    }

  return copy.getLength () == 2 ? 0 : 1;
}
