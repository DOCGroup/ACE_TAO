/**
 * @file XML_Schema_Resolver.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * Resolves schema locations.
 */

#ifndef ACE_XML_SCHEMA_RESOLVER_H
#define ACE_XML_SCHEMA_RESOLVER_H
#include /**/ "ace/pre.h"

#if !defined(ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "XML_Utils_Export.h"
#include "XercesString.h"

#include <vector>
#include <xercesc/sax/EntityResolver.hpp>

namespace XML
{
  // forward decl.
  struct NoOp_Resolver;

  /**
   * @class XML_Schema_Resolver
   * @brief Resolves schema locations
   *
   * Resolver must accept two XMLCh const pointers and return an XMLCh const
   * pointer or a Xerces-allocated XMLCh pointer. Mutable results are adopted
   * and released by XStr.
   */
  template <typename Resolver = NoOp_Resolver>
  class XML_Schema_Resolver : public virtual xercesc::EntityResolver
  {
  public:
    XML_Schema_Resolver () = default;

    explicit XML_Schema_Resolver (Resolver &resolver);

    /// This function is called by the Xerces infrastructure to
    /// actually resolve the location of a schema.
    xercesc::InputSource *resolveEntity (XMLCh const *const publicId, XMLCh const *const systemId) override;

    Resolver &get_resolver ();

  private:
    XML_Schema_Resolver (XML_Schema_Resolver const &) = delete;
    XML_Schema_Resolver &operator= (XML_Schema_Resolver const &) = delete;
    XML_Schema_Resolver (XML_Schema_Resolver &&) = delete;
    XML_Schema_Resolver &operator= (XML_Schema_Resolver &&) = delete;

    Resolver resolver_{};
  };

  /**
   * @class NoOp_Resolver
   * @brief Resolver that does nothing.
   */
  struct NoOp_Resolver
  {
    XMLCh const *operator() (XMLCh const *const, XMLCh const *const systemId) const
    {
      return systemId;
    }
  };

  /**
   * @class Basic_Resolver
   * @brief Resolves a schema location from a fixed path.
   */
  struct Basic_Resolver
  {
    explicit Basic_Resolver (ACE_TCHAR const *path);

    XMLCh *operator() (XMLCh const *const publicId, XMLCh const *const systemId) const;
    XStr path_;
  };

  /**
   * @class Environment_Resolver
   * @brief Resolves a schema location from a path from an environment variable.
   */
  struct XML_Utils_Export Environment_Resolver
  {
    Environment_Resolver () = default;

    Environment_Resolver (ACE_TCHAR const *variable, ACE_TCHAR const *path);

    void add_path (ACE_TCHAR const *variable, ACE_TCHAR const *path);

    XMLCh *operator() (XMLCh const *const publicId, XMLCh const *const systemId) const;

    std::vector<XStr> paths_;
  };
}

#include "XML_Schema_Resolver.tpp"

#include /**/ "ace/post.h"

#endif /*  ACE_XML_SCHEMA_RESOLVER_H */
