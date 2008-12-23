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
#include "XercesString.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/dom/DOMEntityResolver.hpp>
#include <xercesc/dom/DOMInputSource.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/Wrapper4InputSource.hpp>

#include <vector>
#include <string>

using xercesc::DOMEntityResolver;
using xercesc::DOMInputSource;
using xercesc::Wrapper4InputSource;
using xercesc::LocalFileInputSource;

namespace CIAO
{
  namespace Config_Handlers
  {
    // forward decl.
    struct Environment_Resolver;

    /**
     * @class CIAO_Schema_Resolver
     * @brief Resolves schema locations for CIAO.
     *
     * Template argument Resolver_Functor should be a functor with an operation
     * const ACE_TCHAR * operator () (...arguments from resolveEntity...)
     */
    template <typename Resolver_Functor = Environment_Resolver>
    class CIAO_XML_Utils_Export CIAO_Schema_Resolver
      : public virtual DOMEntityResolver
    {
    public:
      /// This function is called by the Xerces infrastructure to
      /// actually resolve the location of a schema.
      virtual DOMInputSource * resolveEntity (const XMLCh *const publicId,
                                              const XMLCh *const systemId,
                                              const XMLCh *const baseURI)
      {
        XStr path = this->resolver_ (publicId, systemId, baseURI);
        if (path.begin () == 0)
          return 0;

        // Ownership of these objects is given to other people.
        return new Wrapper4InputSource (new LocalFileInputSource (path));
      }

      Resolver_Functor &resolver ()
      {
        return this->resolver_;
      }

    private:
      Resolver_Functor resolver_;
    };

    /**
     * @class NoOp_Resolver
     * @brief Resolver that does nothing.
     */
    struct CIAO_XML_Utils_Export NoOp_Resolver
    {
      const XMLCh* operator() (const XMLCh *const,
                               const XMLCh *const systemId,
                               const XMLCh *const)
      {
        return systemId;
      }
    };

    /**
     * @class Basic_Resolver
     * @brief Resolves a schema location from a fixed path.
     *        By default @param path is the current folder.
     */
    class CIAO_XML_Utils_Export Basic_Resolver
    {
    public:
      Basic_Resolver (const ACE_TCHAR *path = ACE_TEXT ("./"));

      XMLCh* operator() (const XMLCh *const publicId,
                         const XMLCh *const systemId,
                         const XMLCh *const baseURI) const;

      void set_path (const ACE_TCHAR *path);

    private:
      XStr path_;
    };

    /**
     * @class Environment_Resolver
     * @brief Resolves a schema location from a path from an
     *        environment variable. By default @param variable is $CIAO_ROOT
     *        and @param path is /docs/schema.
     */
    class CIAO_XML_Utils_Export Environment_Resolver
    {
    public:
      Environment_Resolver (const ACE_TCHAR *variable = ACE_TEXT ("CIAO_ROOT"),
                            const ACE_TCHAR *path = ACE_TEXT ("/docs/schema/"));

      ~Environment_Resolver ();

      void add_path (const ACE_TCHAR *variable,
                     const ACE_TCHAR *path);

      XMLCh* operator() (const XMLCh *const publicId,
                         const XMLCh *const systemId,
                         const XMLCh *const baseURI) const;

    private:
      std::vector<XStr> paths_;
    };
  }
}

#include /**/ "ace/post.h"

#endif /*  CIAO_CONFIG_HANDLERS_XML_SCHEAM_RESOLVER_H */
