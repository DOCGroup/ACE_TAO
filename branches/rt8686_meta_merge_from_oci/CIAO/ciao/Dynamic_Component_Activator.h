// -*- C++ -*-

//=============================================================================
/**
 *  @file    Dynamic_Component_Activator.h
 *
 *  $Id$
 *
 *  @authors Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *           Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_DYNAMIC_SWAPPING_ACTIVATOR_H
#define CIAO_DYNAMIC_SWAPPING_ACTIVATOR_H
#include /**/ "ace/pre.h"

#include "ciao/CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/CCM_ContainerC.h"
#include "ciao/Deployment_CoreC.h"
#include "ciao/Dynamic_Component_Servant_Base.h"

#include "tao/PortableServer/Key_Adapters.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/ServantActivatorC.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Array_Base.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace CIAO
{

  class Dynamic_Component_Servant_Base;
  /**
   * @class Dynamic_Component_Activator
   *
   * @brief Activator that is registered with the POA for facet and
   * consumer servants.
   *
   */
  class CIAO_SERVER_Export Dynamic_Component_Activator
    : public virtual PortableServer::ServantActivator
    , public virtual TAO_Local_RefCounted_Object
  {
  public:
    Dynamic_Component_Activator (CORBA::ORB_ptr o);

    virtual ~Dynamic_Component_Activator (void);

    /// Template methods overridden to get callbacks.
    /**
     * If you would like to know the details of the following two
     * methods, please PortableServer documentation. This is probably
     * not the place to document what these mean.
     */
    PortableServer::Servant incarnate (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa
      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableServer::ForwardRequest));

    void etherealize (const PortableServer::ObjectId &oid,
                      PortableServer::POA_ptr adapter,
                      PortableServer::Servant servant,
                      CORBA::Boolean cleanup_in_progress,
                      CORBA::Boolean remaining_activations
                      ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void add_servant_map (PortableServer::ObjectId &oid,
                          Dynamic_Component_Servant_Base* servant);

    void delete_servant_map (PortableServer::ObjectId &oid);

  private:
    /// Pointer to our ORB
    CORBA::ORB_var orb_;

    ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                            Dynamic_Component_Servant_Base *,
                            TAO_ObjectId_Hash,
                            ACE_Equal_To<PortableServer::ObjectId>,
                            ACE_SYNCH_MUTEX>
      servant_map_;

  };
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /*CIAO_DYNAMIC_SWAPPING_ACTIVATOR_H*/
