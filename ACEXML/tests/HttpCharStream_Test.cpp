// $Id$

#include "ACEXML/common/HttpCharStream.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  const ACEXML_Char* godfather = "http://www.cs.wustl.edu/~nanbor/";

  const ACEXML_Char* shishya = "http://www.cs.wustl.edu/~kitty/";

  ACEXML_HttpCharStream stream (godfather);

  ACEXML_Char ch;
  while (stream.get (ch) != -1)
    ACE_OS::printf ("%c", ch);

  stream.close();

  if (stream.open (shishya) != -1)
    {
      while (stream.get (ch) != -1)
        ACE_OS::printf ("%c", ch);
    }
}
