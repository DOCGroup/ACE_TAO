/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerAdmin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   Define the interface for the RtecEventChannelAdmin::ConsumerAdmin
//   implementations.
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_CONSUMERADMIN_H
#define TAO_EC_CONSUMERADMIN_H

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event/EC_Busy_Lock.h"
#include "orbsvcs/Event/EC_Filter.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;
template<class Target,class Object> class TAO_EC_Connected_Command;
template<class Target,class Object> class TAO_EC_Disconnected_Command;

class TAO_EC_ConsumerAdmin : public POA_RtecEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   ConsumerAdmin
  //
  // = DESCRIPTION
  //   Implements the ConsumerAdmin interface, i.e. the factory for
  //   ProxyPushSupplier objects.
  //
  // = MEMORY MANAGMENT
  //   It does not assume ownership of the TAO_EC_Event_Channel object
  //
  // = LOCKING
  //   No provisions for locking, access must be serialized
  //   externally.
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerAdmin (TAO_EC_Event_Channel* event_channel);
  // constructor...

  virtual ~TAO_EC_ConsumerAdmin (void);
  // destructor...

  typedef ACE_Unbounded_Set<TAO_EC_ProxyPushSupplier*> SupplierSet;
  typedef ACE_Unbounded_Set_Iterator<TAO_EC_ProxyPushSupplier*> SupplierSetIterator;

  virtual int busy (void) = 0;
  virtual int idle (void) = 0;
  // Before using the iterators the clients should invoke this
  // methods, that ensures that no changes to the underlying data
  // structure will occur.

  void busy_hwm (CORBA::ULong hwm);
  CORBA::ULong busy_hwm (void) const;
  // This attribute is used to control the maximum number of threads
  // that can be running on the

  SupplierSetIterator begin (void);
  SupplierSetIterator end (void);
  // Iterators over the set of ProxyPushSuppliers

  void set_default_POA (PortableServer::POA_ptr poa);
  // Set this servant's default POA

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment& env);
  // Override the ServantBase method.

  virtual void connected (TAO_EC_ProxyPushConsumer*,
                          CORBA::Environment&);
  virtual void disconnected (TAO_EC_ProxyPushConsumer*,
                             CORBA::Environment&);
  // Used to inform the EC that a Consumer has connected or
  // disconnected from it.

  virtual void connected (TAO_EC_ProxyPushSupplier*,
                          CORBA::Environment&) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier*,
                             CORBA::Environment&) = 0;
  // Used to inform the EC that a Supplier has connected or
  // disconnected from it.

  // = The RtecEventChannelAdmin::ConsumerAdmin methods...
  virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
      obtain_push_supplier (CORBA::Environment &);

  typedef TAO_EC_Busy_Lock_Adapter<TAO_EC_ConsumerAdmin> Busy_Lock;
  Busy_Lock& busy_lock (void);
  // This object is an adapter to the busy/idle protocol.

protected:
  virtual void connected_i (TAO_EC_ProxyPushSupplier* supplier,
                            CORBA::Environment &env);
  // The implementation of connected(), without locking.
  // It does not increase the reference count on the supplier

  virtual void disconnected_i (TAO_EC_ProxyPushSupplier* supplier,
                               CORBA::Environment &env);
  // The implementation of disconnected(), without locking.
  // It decreases the reference count on the supplier if the operation 
  // is successful.

  typedef TAO_EC_Connected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier> Connected_Command;
  typedef TAO_EC_Connected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier> Disconnected_Command;

  friend class TAO_EC_Connected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>;
  friend class TAO_EC_Disconnected_Command<TAO_EC_ConsumerAdmin,TAO_EC_ProxyPushSupplier>;
  // This two classes call the connected_i() and disconnected_i()
  // methods, that's ok because they do while this class is holding
  // its lock.

  virtual void execute_delayed_operations (void);
  // Dervied classes that implement delayed disconnects and connects
  // must override this method.

  SupplierSet all_suppliers_;
  // The set of all the ProxyPushSupplier objects bound to this
  // ConsumerAdmin.

  TAO_EC_Busy_Lock_Adapter<TAO_EC_ConsumerAdmin> busy_lock_;
  // The busy lock object

private:
  TAO_EC_Event_Channel *event_channel_;
  // The Event Channel we belong to

  PortableServer::POA_var default_POA_;
  // Store the default POA.

  CORBA::ULong busy_hwm_;
  // How many threads can simultaneously iterate over the set.
};

#if defined (__ACE_INLINE__)
#include "EC_ConsumerAdmin.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_CONSUMERADMIN_H */
