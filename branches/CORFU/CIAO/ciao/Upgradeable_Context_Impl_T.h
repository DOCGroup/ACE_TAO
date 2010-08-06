// -*- C++ -*-

//=============================================================================
/**
 *  @file    Upgradeable_Context_Impl_T.h
 *
 *  $Id$
 *
 */
//=============================================================================


#ifndef CIAO_UPGRADEABLE_CONTEXT_IMPL_T_H
#define CIAO_UPGRADEABLE_CONTEXT_IMPL_T_H

#include /**/ "ace/pre.h"

#include "ciao/CIAO_UpgradeableContextC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Context_Impl_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
namespace CORBA
{
  class SystemException;
}
TAO_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  class Upgradeable_Context_Impl
    : public virtual Context_Impl<BASE_CTX, SVNT, COMP>
  {
  public:
    Upgradeable_Context_Impl (Components::CCMHome_ptr the_home,
                              Session_Container *c,
                              SVNT *sv);

    virtual ~Upgradeable_Context_Impl (void);

    virtual void
    deactivate_facet (const PortableServer::ObjectId &oid);

    virtual void
    update_port_activator (const PortableServer::ObjectId &oid);

    virtual void
    remove_facet (CORBA::Object_ptr reference);

    virtual ::Components::ConsumerDescriptions *
    get_registered_consumers (const char *publisher_name) = 0;

  private:
    /// Not to be used
    Upgradeable_Context_Impl (void);
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Upgradeable_Context_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Upgradeable_Context_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_UPGRADEABLE_CONTEXT_IMPL_T_H */
