// $Id$

/**
 * @file HomeRegistrar_i.h
 *
 * A CIAO HomeFinder implementation that also support CIAO's home
 * registration operation.
 *
 * @sa HomeRegistrar.idl
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_HOMEREGISTRAR_I_H
#define CIAO_HOMEREGISTRAR_I_H
#include "ace/pre.h"

#include "HomeRegistrarS.h"
#include <CosNamingC.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class CIAO_SERVER_Export HomeRegistrar_Impl
    : public virtual POA_CIAO::HomeRegistrar,
      public virtual PortableServer::RefCountServantBase
  {
  public:
    /**
     * @brief We require a NamingContext to hold the names.
     */
    HomeRegistrar_Impl (CosNaming::NamingContext_ptr ns);

    /**
     * @brief Default destructor.
     */
    ~HomeRegistrar_Impl ();

    virtual ::Components::CCMHome_ptr find_home_by_component_type (const char * comp_repid
                                                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::HomeNotFound));

    virtual ::Components::CCMHome_ptr find_home_by_home_type (const char * home_repid
                                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::HomeNotFound));

    virtual ::Components::CCMHome_ptr find_home_by_name (const char * home_name
                                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::HomeNotFound));

    virtual void register_home (const char * home_repo_id,
                                const char * comp_repo_id,
                                const char * home_name,
                                CORBA::Object_ptr objref
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CIAO::AlreadyBound,
                       CIAO::InvalidName,
                       CIAO::HomeRegisterFailure));

    Components::CCMHome_ptr find_home_impl (const char *name
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::HomeNotFound));

  protected:
    CosNaming::NamingContext_var ns_;
  };
}

#if defined (__ACE_INLINE__)
#include "HomeRegistrar_i.inl"
#endif /* !defined INLINE */

#include "ace/post.h"
#endif /* CIAO_HOMEREGISTRAR_I_H */
