#include "ace/OS_NS_string.h"

#include "ACEXML/common/StreamFactory.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/HttpCharStream.h"
#include <memory>
#include <new>

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
      std::unique_ptr<ACEXML_HttpCharStream> hstream {
        new (std::nothrow) ACEXML_HttpCharStream};
      if (!hstream)
        {
          errno = ENOMEM;
          return nullptr;
        }
      if (hstream->open (uri) != -1)
        return hstream.release ();
    }
  else
    {
      if (ACE_OS::strstr (uri, ACE_TEXT ("file://")) != nullptr)
        uri += 7; // Skip over file://
      std::unique_ptr<ACEXML_FileCharStream> fstream {
        new (std::nothrow) ACEXML_FileCharStream};
      if (!fstream)
        {
          errno = ENOMEM;
          return nullptr;
        }
      if (fstream->open (uri) != -1)
        return fstream.release ();
#ifdef USE_ZZIP
      else
        {
          fstream.reset ();
          std::unique_ptr<ACEXML_ZipCharStream> zstream {
            new (std::nothrow) ACEXML_ZipCharStream};
          if (!zstream)
            {
              errno = ENOMEM;
              return nullptr;
            }
          if (zstream->open (uri) != -1)
            return zstream.release ();
        }
#endif /* USE_ZZIP */
    }
  return nullptr;
}

ACEXML_StreamFactory::~ACEXML_StreamFactory () = default;
