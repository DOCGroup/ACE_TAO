// $Id$

#include "ACEXML/common/HttpCharStream.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  const ACEXML_Char* test = ACE_TEXT("http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd");

  const ACEXML_Char* simple = ACE_TEXT("http://www.w3.org/TR/xhtml1/DTD/xhtml-lat1.ent");

  ACEXML_HttpCharStream first;
  ACEXML_HttpCharStream second;
  ACEXML_Char ch;

  if (first.open (test) != -1)
    {
      while (first.get (ch) != -1)
        ACE_OS::printf ("%c", ch);
    }
  else
    {
      first.close();
      ACE_ERROR ((LM_ERROR, "Error in opening stream : %m\n"));
    }
  if (second.open (simple) != -1)
    {
      while (second.get (ch) != -1)
        ACE_OS::printf ("%c", ch);
    }
  else
    {
      second.close();
      ACE_ERROR ((LM_ERROR, "Error in opening stream : %m\n"));
    }
  first.close();
  second.close();
  return 0;
}
