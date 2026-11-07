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
  if (uri == nullptr)
    return nullptr;
  ACEXML_FileCharStream* fstream = nullptr;
  ACEXML_HttpCharStream* hstream = nullptr;

  if (ACE_OS::strstr (uri, ACE_TEXT("ftp://")) != nullptr)
    {
      return nullptr;
    }
  else if (ACE_OS::strstr (uri, ACE_TEXT ("http://")) != nullptr)
    {
      ACE_NEW_RETURN (hstream, ACEXML_HttpCharStream, nullptr);
      if (hstream->open (uri) != -1)
        return hstream;
    }
  else
    {
      if (ACE_OS::strstr (uri, ACE_TEXT ("file://")) != nullptr)
        uri += 7; // Skip over file://
      ACE_NEW_RETURN (fstream, ACEXML_FileCharStream, nullptr);
      if (fstream->open (uri) != -1)
        return fstream;
#ifdef USE_ZZIP
      else
        {
          ACEXML_ZipCharStream* zstream = nullptr;
          ACE_NEW_RETURN (zstream, ACEXML_ZipCharStream, nullptr);
          if (zstream->open (uri) != -1)
            return zstream;
        }
#endif /* USE_ZZIP */
    }
  return nullptr;
}

ACEXML_StreamFactory::~ACEXML_StreamFactory ()
{
  // No op
}
