#ifndef XML_SCHEMA_RESOLVER_TPP
#define XML_SCHEMA_RESOLVER_TPP

#include "XML_Schema_Resolver.h"
#include "XercesString.h"

#include <xercesc/framework/LocalFileInputSource.hpp>

namespace XML
{
  template <typename Resolver>
  XML_Schema_Resolver<Resolver>::XML_Schema_Resolver (Resolver &res)
    : resolver_ (res)
  {
  }

  template <typename Resolver>
  Resolver &XML_Schema_Resolver<Resolver>::get_resolver ()
  {
    return resolver_;
  }

  /// This function is called by the Xerces infrastructure to
  /// actually resolve the location of a schema.
  template <typename Resolver>
  xercesc::InputSource *XML_Schema_Resolver<Resolver>::resolveEntity (XMLCh const *const publicId,
                                                                      XMLCh const *const systemId)
  {
    XStr path (resolver_ (publicId, systemId));
    if (path.begin () == nullptr)
      return nullptr;

    // Xerces takes ownership of the returned input source.
    return new xercesc::LocalFileInputSource (path.begin ());
  }
}

#endif /*XML_SCHEMA_RESOLVER_TPP*/
