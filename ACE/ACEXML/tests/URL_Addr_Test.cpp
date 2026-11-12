#include "ACEXML/common/URL_Addr.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_main.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACEXML_URL_Addr original;
  if (original.string_to_addr (ACE_TEXT ("http://127.0.0.1:8080/a/long/path")) != 0)
    return 1;

  ACEXML_Char const * const expected_path = ACE_TEXT ("/a/long/path");
  if (ACE_OS::strcmp (original.get_path_name (), expected_path) != 0)
    return 1;

  ACEXML_Char const * const original_text = original.addr_to_string (1);
  if (original_text == nullptr)
    return 1;

  ACEXML_URL_Addr copy (original);
  ACEXML_Char const * const copied_text = copy.addr_to_string (1);
  if (ACE_OS::strcmp (copy.get_path_name (), expected_path) != 0 ||
      copied_text == nullptr || ACE_OS::strcmp (copied_text, original_text) != 0)
    return 1;

  if (copy.set (copy) != 0)
    return 1;

  ACEXML_URL_Addr assigned;
  assigned = original;
  assigned = assigned;
  ACEXML_Char const * const assigned_path = assigned.get_path_name ();
  return assigned_path != nullptr &&
         ACE_OS::strcmp (assigned_path, expected_path) == 0 ? 0 : 1;
}
