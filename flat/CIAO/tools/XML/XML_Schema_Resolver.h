/**
 * @file XML_Schema_Resolver.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Resolves schema locations.
 */

#ifndef CIAO_XML_SCHEMA_RESOLVER_H
#define CIAO_XML_SCHEMA_RESOLVER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML/CIAO_XML_Utils_Export.h"
#include "XML/XercesString.h"

#include <xercesc/dom/DOMEntityResolver.hpp>
#include <xercesc/dom/DOMInputSource.hpp>

#include <vector>
#include <string>

using xercesc::DOMEntityResolver;
using xercesc::DOMInputSource;

namespace CIAO
{
  namespace XML
  {
    // forward decl.
    struct NoOp_Resolver;

    /**
     * @class CIAO_Schema_Resolver
     * @brief Resolves schema locations for CIAO.
     *
     * Template argument Resolver should be a functor with an operation
     * const ACE_TCHAR * operator () (...arguments from resolveEntity...)
     */
    template <typename Resolver = NoOp_Resolver>
    class XML_Schema_Resolver
      : public virtual DOMEntityResolver
    {
    public:
      XML_Schema_Resolver (void);
      
      XML_Schema_Resolver (Resolver &resolver);

      /// This function is called by the Xerces infrastructure to
      /// actually resolve the location of a schema.
      virtual DOMInputSource * resolveEntity (const XMLCh *const publicId,
                                              const XMLCh *const systemId,
                                              const XMLCh *const baseURI);

    private:
      XML_Schema_Resolver (XML_Schema_Resolver<Resolver> &);

      Resolver &resolver_;
    };
    
    /**
     * @class NoOp_Resolver
     * @brief Resolver that does nothing.
     */
    struct NoOp_Resolver
    {
      const XMLCh* operator() (const XMLCh *const,
                               const XMLCh *const systemId,
                               const XMLCh *const)
      { return systemId; };
    };

    /**
     * @class Basic_Resolver
     * @brief Resolves a schema location from a fixed path.
     */
    struct Basic_Resolver
    {
      Basic_Resolver (const ACE_TCHAR *path);
      
      XMLCh* operator() (const XMLCh *const publicId,
                               const XMLCh *const systemId,
                               const XMLCh *const baseURI) const;
      XStr path_;
    };
    
    /**
     * @class Environment_Resolver
     * @brief Resolves a schema location from a path from an environment variable.
     */
    struct CIAO_XML_Utils_Export Environment_Resolver
    {
      Environment_Resolver (const ACE_TCHAR *variable = "",
                            const ACE_TCHAR *path = "./");
      
      void add_path (const ACE_TCHAR *variable, 
                     const ACE_TCHAR *path);
      
      XMLCh* operator() (const XMLCh *const publicId,
                         const XMLCh *const systemId,
                         const XMLCh *const baseURI) const;
      
      std::vector<XStr> paths_;
    };
  }
}

#include "XML/XML_Schema_Resolver.tpp"

#include /**/ "ace/post.h"

#endif /*  CIAO_XML_SCHEMA_RESOLVER_H */
