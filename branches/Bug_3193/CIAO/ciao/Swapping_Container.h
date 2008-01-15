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

#include "ciao/CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Session_Container.h"

namespace CIAO
{
  class Dynamic_Component_Servant_Base;
  class Dynamic_Component_Activator;

  class CIAO_SERVER_Export Swapping_Container : public Session_Container
  {
  public:
    Swapping_Container (CORBA::ORB_ptr o, Container_Impl *container_impl);

    virtual ~Swapping_Container (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0);

    virtual CORBA::Object_ptr install_servant (PortableServer::Servant p,
                                               Container::OA_Type t);

    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p);

    virtual void ciao_uninstall_home (Components::CCMHome_ptr homeref);

    virtual CORBA::Object_ptr get_home_objref (PortableServer::Servant p);

    virtual void add_servant_to_map (PortableServer::ObjectId &oid,
                                  Dynamic_Component_Servant_Base* servant);

    virtual void delete_servant_from_map (PortableServer::ObjectId &oid);

    virtual void deactivate_facet (const PortableServer::ObjectId &oid);

    PortableServer::POA_ptr the_home_servant_POA (void) const;

  private:
    /// Create POA  for the component.
    void create_servant_POA (const char *name,
                             const CORBA::PolicyList *p,
                             PortableServer::POA_ptr root);

    void create_home_servant_POA (const char *name,
                                  const CORBA::PolicyList *p,
                                  PortableServer::POA_ptr root);

    void create_connections_POA (PortableServer::POA_ptr root);
    
  protected:
    unsigned long number_;

    /// Static variable to store the highest number we have given out until
    /// now
    static ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long> serial_number_;

    Dynamic_Component_Activator *dsa_;
  };
}

// Macro for registration of an OBV factory in the generated
// servant class. Similar to the macro for TAO in
// tao/ValueType/ValueFactory.h but here we take advantage of
// the fact that we have access to the current ORB indirectly
// through the context and container.
// @@Jai, is this macro required here?
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
