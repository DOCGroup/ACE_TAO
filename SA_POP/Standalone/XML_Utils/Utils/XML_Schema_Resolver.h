/**
 * @file XML_Schema_Resolver.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Resolves schema locations.
 */

#ifndef CIAO_CONFIG_HANDLERS_XML_SCHEAM_RESOLVER_H
#define CIAO_CONFIG_HANDLERS_XML_SCHEAM_RESOLVER_H
#include /**/ "ace/pre.h"

#include "CIAO_XML_Utils_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/dom/DOMEntityResolver.hpp>
#include <xercesc/dom/DOMInputSource.hpp>

#include <string>

using xercesc::DOMEntityResolver;
using xercesc::DOMInputSource;

namespace CIAO
{
  namespace Config_Handlers
  {
    /**
     * @class CIAO_Schema_Resolver
     * @brief Resolves schema locations for CIAO.
     */
    class CIAO_Schema_Resolver
      : public virtual DOMEntityResolver
    {
    public:
      /// This constructor locates the schema in the default location
      /// from CIAO_ROOT/docs/schema.
      CIAO_Schema_Resolver (void);

      /// If this constructor is passed a nonempty string, it locates
      /// the schema in that path, otherwise its behavior is the same
      /// as the default constructor.
      CIAO_Schema_Resolver (const char *path);

      /// This function is called by the Xerces infrastructure to
      /// actually resolve the location of a schema.
      virtual DOMInputSource * resolveEntity (const XMLCh *const publicId,
                                              const XMLCh *const systemId,
                                              const XMLCh *const baseURI);

    private:
      std::string resolve_from_environment (void);

      std::string base_path_;
    };
  }
}


#include /**/ "ace/post.h"

#endif /*  CIAO_CONFIG_HANDLERS_XML_SCHEAM_RESOLVER_H */
