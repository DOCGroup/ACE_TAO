// -*- C++ -*-

//=============================================================================
/**
 *  @file    Swapping_Container.h
 *
 *  $Id$
 *
 *  Header file for CIAO's Dynamic Swapping container implementations
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *          Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_SWAPPING_CONTAINER_H
#define CIAO_SWAPPING_CONTAINER_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ciao/CCM_ContainerC.h"
#include "ciao/Container_Base.h"
#include "ciao/Deployment_CoreC.h"
#include "ciao/Dynamic_Component_Activator.h"
#include "ciao/CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class CIAO_SERVER_Export Swapping_Container : public Session_Container
  {
  public:

    Swapping_Container (CORBA::ORB_ptr o);

    virtual ~Swapping_Container (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:

    /// Create POA  for the component.
    void create_component_POA (const char *name,
                               const CORBA::PolicyList *p,
                               PortableServer::POA_ptr root
                               ACE_ENV_ARG_DECL);
  protected:
    long number_;

    static ACE_Atomic_Op <ACE_SYNCH_MUTEX, long> serial_number_;

    Dynamic_Component_Activator *dsa_;

  };
}

// Macro for registration of an OBV factory in the generated
// servant class. Similar to the macro for TAO in
// tao/ValueType/ValueFactory.h but here we take advantage of
// the fact that we have access to the current ORB indirectly
// through the context and container.
#define CIAO_REGISTER_OBV_FACTORY(FACTORY, VALUETYPE) \
  {  \
    CORBA::ValueFactory factory = new FACTORY; \
    CORBA::ORB_ptr orb = \
      this->context_->_ciao_the_Container ()->the_ORB (); \
    CORBA::ValueFactory prev_factory = \
      orb->register_value_factory ( \
               VALUETYPE::_tao_obv_static_repository_id (), \
               factory); \
    CORBA::remove_ref (prev_factory); \
    CORBA::add_ref (factory);  \
  }

#if defined (__ACE_INLINE__)
# include "Swapping_Container.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_SWAPPING_CONTAINER_H */
