// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Activator.h
 *
 *  $Id$
 *
 *  @authors Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_SERVANT_ACTIVATOR_H
#define CIAO_SERVANT_ACTIVATOR_H

#include /**/ "ace/pre.h"

#include "CIAO_Servant_Impl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/Array_Base.h>
#include <tao/PortableServer/ServantActivatorC.h>
#include <tao/LocalObject.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

namespace CIAO
{
  class Port_Activator;

  /**
   * @class Servant_Activator
   *
   * @brief Activator that is registered with the POA for facet and
   * consumer servants.
   *
   * This class acts like a factory in some sense. This factory is
   * registered with the POA with RETAIN policy. When the factory gets
   * a call back as part of the upcall, this factory looks at the
   * list of port activators registered, uses the OID to pick the
   * right one (yes a linear algorithm is used), calls activate () on
   * it which returns the servant for *that* port.
   */
  class CIAO_Servant_Impl_Export Servant_Activator
    : public virtual PortableServer::ServantActivator,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    Servant_Activator (CORBA::ORB_ptr o);

    virtual ~Servant_Activator (void);

    bool update_port_activator (const PortableServer::ObjectId &oid);

    /// Template methods overridden to get callbacks.
    /**
     * If you would like to know the details of the following two
     * methods, please PortableServer documentation. This is probably
     * not the place to document what these mean.
     */
    virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
					       PortableServer::POA_ptr poa);

    virtual void etherealize (const PortableServer::ObjectId &oid,
			      PortableServer::POA_ptr adapter,
			      PortableServer::Servant servant,
			      CORBA::Boolean cleanup_in_progress,
			      CORBA::Boolean remaining_activations);

    /// Local helper methods
    bool register_port_activator (Port_Activator *pa);

  private:
    /// Pointer to our ORB
    CORBA::ORB_var orb_;

    /// @@ This should be changed at some point of time so that we
    /// don't  land up with a linear algorithm
    typedef ACE_Array_Base<Port_Activator *> Port_Activators;

    /// Array of port activators
    Port_Activators pa_;

    /// Running index of the slot that has been just filled in.
    size_t slot_index_;

    /// Mutex that synchronizes access to the array.
    TAO_SYNCH_MUTEX  mutex_;
  };
}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /*CIAO_SERVANT_ACTIVATOR_H*/
