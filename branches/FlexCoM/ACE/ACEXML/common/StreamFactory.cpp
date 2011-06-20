// $Id$

#include "ace/OS_NS_string.h"

#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"

#ifdef USE_ZZIP
#include "ACEXML/common/ZipCharStream.h"
#endif /* USE_ZZIP */



ACEXML_CharStream*
ACEXML_StreamFactory::create_stream (const ACEXML_Char* uri)
{
  if (uri == 0)
    return 0;
  ACEXML_FileCharStream* fstream = 0;
  ACEXML_HttpCharStream* hstream = 0;

  if (ACE_OS::strstr (uri, ACE_TEXT("ftp://")) != 0)
    {
      return 0;
    }
  else if (ACE_OS::strstr (uri, ACE_TEXT ("http://")) != 0)
    {
      ACE_NEW_RETURN (hstream, ACEXML_HttpCharStream, 0);
      if (hstream->open (uri) != -1)
        return hstream;
    }
  else
    {
      if (ACE_OS::strstr (uri, ACE_TEXT ("file://")) != 0)
        uri += 7; // Skip over file://
      ACE_NEW_RETURN (fstream, ACEXML_FileCharStream, 0);
      if (fstream->open (uri) != -1)
        return fstream;
#ifdef USE_ZZIP
      else
        {
          ACEXML_ZipCharStream* zstream = 0;
          ACE_NEW_RETURN (zstream, ACEXML_ZipCharStream, 0);
          if (zstream->open (uri) != -1)
            return zstream;
        }
#endif /* USE_ZZIP */
    }
  return 0;
}

ACEXML_StreamFactory::~ACEXML_StreamFactory ()
{
  // No op
}
