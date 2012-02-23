// -*- C++ -*-

//=============================================================================
/**
 *  @file    Session_Container.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_SESSION_CONTAINER_H
#define CIAO_SESSION_CONTAINER_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Session/Session_Container_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Containers/Container_Base_T.h"
#include "ciao/Containers/Session/Session_ContainerC.h"
#include "ccm/CCM_HomeExecutorBaseC.h"
#include "ccm/CCM_EnterpriseComponentC.h"

namespace CIAO
{
  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*HomeServantFactory) (::Components::HomeExecutorBase_ptr p,
                                                           ::CIAO::Session_Container_ptr c,
                                                           const char *ins_name);
  typedef ::Components::EnterpriseComponent_ptr (*ComponentFactory) (void);
  typedef ::PortableServer::Servant (*ComponentServantFactory) (::Components::EnterpriseComponent_ptr,
                                                                ::CIAO::Session_Container_ptr,
                                                                const char *);

  class SESSION_CONTAINER_Export Session_Container_i :
    public Container_i < ::CIAO::Session_Container>
  {
  public:
    Session_Container_i (CORBA::ORB_ptr o,
                         PortableServer::POA_ptr poa);

    virtual ~Session_Container_i (void);

    /// Initialize the container with a name.
    virtual void init (const char *name);

    virtual void fini (void);

    /**
     * @brief Simply installing a home executor into the component.
     *
     * This operation install a home executor into the component.  It
     * requires the name of the DLLs to executor and the servant glue
     * code, and the entry points to the respective DLLs.  Currently,
     * we don't try to manage the lifetime of DLL objects, but we
     * should at some later point.
     *
     * @retval Home objref of the installed home.
     */
    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *servant_artifact,
                                                  const char *servant_entrypoint,
                                                  const char *name,
                                                  CORBA::Long open_mode);

    /// Install a new component
    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name,
                                                         CORBA::Long open_mode);

  private:
    /// Not allowed to be
    Session_Container_i (void);
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_SESSION_CONTAINER_H */
