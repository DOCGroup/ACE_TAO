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

#include "CIAO_Servant_Activator_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"
#include "ciao/Containers/CIAO_Servant_ActivatorC.h"

#include <map>
#include <string>

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
  class CIAO_SERVANT_ACTIVATOR_Export Servant_Activator_i
    : public virtual ::CIAO::Servant_Activator,
      public virtual ::CORBA::LocalObject
  {
  public:
    Servant_Activator_i (CORBA::ORB_ptr o);

    virtual ~Servant_Activator_i (void);

    virtual ::CORBA::Boolean update_port_activator (
      const ::PortableServer::ObjectId &oid);

    virtual ::CORBA::Boolean register_port_activator (
      ::CIAO::Port_Activator_ptr pa);

    /// Template methods overridden to get callbacks.
    /**
     * If you would like to know the details of the following two
     * methods, please PortableServer documentation. This is probably
     * not the place to document what these mean.
     */
    virtual PortableServer::Servant incarnate (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa);

    virtual void etherealize (const PortableServer::ObjectId &oid,
                              PortableServer::POA_ptr adapter,
                              PortableServer::Servant servant,
                              CORBA::Boolean cleanup_in_progress,
                              CORBA::Boolean remaining_activations);

  private:
    /// Pointer to our ORB
    CORBA::ORB_var orb_;

    typedef std::map <std::string, Port_Activator_var> Port_Activators;

    /// Array of port activators
    Port_Activators pa_;

    /// Running index of the slot that has been just filled in.
    size_t slot_index_;

    /// Mutex that synchronizes access to the array.
    TAO_SYNCH_MUTEX  mutex_;
  };
}

#include /**/ "ace/post.h"

#endif /*CIAO_SERVANT_ACTIVATOR_H*/
