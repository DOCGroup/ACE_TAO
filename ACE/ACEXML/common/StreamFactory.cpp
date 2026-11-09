#include "ace/OS_NS_string.h"

#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"
#include <memory>

#ifdef USE_ZZIP
#include "ACEXML/common/ZipCharStream.h"
#endif /* USE_ZZIP */


ACEXML_CharStream*
ACEXML_StreamFactory::create_stream (ACEXML_Char const *uri)
{
  if (uri == nullptr)
    return nullptr;
  if (ACE_OS::strstr (uri, ACE_TEXT("ftp://")) != nullptr)
    {
      return nullptr;
    }
  else if (ACE_OS::strstr (uri, ACE_TEXT ("http://")) != nullptr)
    {
      ACEXML_HttpCharStream *raw_stream {};
      ACE_NEW_RETURN (raw_stream, ACEXML_HttpCharStream, nullptr);
      std::unique_ptr<ACEXML_HttpCharStream> hstream (raw_stream);
      if (hstream->open (uri) != -1)
        return hstream.release ();
    }
  else
    {
      if (ACE_OS::strstr (uri, ACE_TEXT ("file://")) != nullptr)
        uri += 7; // Skip over file://
      ACEXML_FileCharStream *raw_stream {};
      ACE_NEW_RETURN (raw_stream, ACEXML_FileCharStream, nullptr);
      std::unique_ptr<ACEXML_FileCharStream> fstream (raw_stream);
      if (fstream->open (uri) != -1)
        return fstream.release ();
#ifdef USE_ZZIP
      else
        {
          fstream.reset ();
          ACEXML_ZipCharStream *raw_zip_stream {};
          ACE_NEW_RETURN (raw_zip_stream, ACEXML_ZipCharStream, nullptr);
          std::unique_ptr<ACEXML_ZipCharStream> zstream (raw_zip_stream);
          if (zstream->open (uri) != -1)
            return zstream.release ();
        }
#endif /* USE_ZZIP */
    }
  return nullptr;
}

ACEXML_StreamFactory::~ACEXML_StreamFactory () = default;
