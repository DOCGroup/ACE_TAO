// $Id$

#include "ACEXML/common/HttpCharStream.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  const ACEXML_Char* test = ACE_TEXT("http://www.cs.wustl.edu/~nanbor/");

  const ACEXML_Char* simple = ACE_TEXT("http://www.cs.wustl.edu/~kitty/");

  ACEXML_HttpCharStream stream;
  ACEXML_Char ch;

  if (stream.open (test) != -1)
    {
      while (stream.get (ch) != -1)
        ACE_OS::printf ("%c", ch);
      stream.close();
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Error in opening stream : %m\n"));
    }

  if (stream.open (simple) != -1)
    {
      while (stream.get (ch) != -1)
        ACE_OS::printf ("%c", ch);
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Error in opening stream : %m\n"));
    }
  return 0;
}
