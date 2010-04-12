// $Id$

#include "ACEXML/common/XML_Util.h"

static const ACEXML_Char ESCAPED_AMP[] = ACE_TEXT("&amp;");
static const ACEXML_Char ESCAPED_LESS[] = ACE_TEXT("&lt;");
static const ACEXML_Char ESCAPED_GREATER[] = ACE_TEXT("&gt;");
static const ACEXML_Char ESCAPED_APOS[] = ACE_TEXT("&apos;");
static const ACEXML_Char ESCAPED_QUOTE[] = ACE_TEXT("&quot;");

#define CSTRLEN(x) ((sizeof(x) / sizeof(ACEXML_Char)) - 1)

ACEXML_String ACEXML_escape_string(const ACEXML_String& str)
{
  ACEXML_String ret(str.length ());
  ACEXML_escape_string(str, ret);
  return ret;
}

void ACEXML_escape_string(const ACEXML_String& in, ACEXML_String& out)
{
  size_t len = in.length ();
  out.clear();
  for (size_t stridx = 0; stridx < len; ++stridx)
  {
    switch (in[stridx]) {
    case '&':
      out.append(ESCAPED_AMP, CSTRLEN(ESCAPED_AMP));
      break;
    case '<':
      out.append(ESCAPED_LESS, CSTRLEN(ESCAPED_LESS));
      break;
    case '>':
      out.append(ESCAPED_GREATER, CSTRLEN(ESCAPED_GREATER));
      break;
    case '\'':
      out.append(ESCAPED_APOS, CSTRLEN(ESCAPED_APOS));
      break;
    case '\"':
      out.append(ESCAPED_QUOTE, CSTRLEN(ESCAPED_QUOTE));
      break;
    default:
      out += in[stridx];
    }
  }
}
