// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Server_Strategy_Factory.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#ifndef TAO_SERVER_STRATEGY_FACTORY_H
#  define TAO_SERVER_STRATEGY_FACTORY_H

class TAO_Server_Strategy_Factory : public ACE_Service_Object
{
  // = TITLE
  //    Base class for the server's abstract factory that manufactures
  //    various strategies of special utility to it.  This simply
  //    serves as an interface to a subclass that REALLY gets
  //    specified and loaded by the Service Configurator.
public:
  // = Initialization and termination methods.
  TAO_Server_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Server_Strategy_Factory(void);
  // Destructor.

  virtual int open (void);
  // Call <open> on various strategies.  This is not performed in
  // <init> so that the other portions of the ORB have a chance to
  // "settle" in their initialization since the streategies herein
  // might need some of that information.

  // = Server-side factory types.
  typedef ACE_Creation_Strategy<TAO_Server_Connection_Handler>
          CREATION_STRATEGY;
  typedef ACE_Accept_Strategy<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
          ACCEPT_STRATEGY;
  typedef ACE_Concurrency_Strategy<TAO_Server_Connection_Handler>
          CONCURRENCY_STRATEGY;
  typedef ACE_Scheduling_Strategy<TAO_Server_Connection_Handler>
          SCHEDULING_STRATEGY;

  // = Factory methods for the server-side strategies.

  virtual CREATION_STRATEGY *creation_strategy (void);
  // Return concrete creation strategy.

  virtual ACCEPT_STRATEGY *accept_strategy (void);
  // Return concrete acceptor strategy.

  virtual CONCURRENCY_STRATEGY *concurrency_strategy (void);
  // Return the concurrency strategy used.

  virtual SCHEDULING_STRATEGY *scheduling_strategy (void);
  // Return the scheduling strategy used.

  virtual TAO_Object_Table_Impl *create_object_table (int user_id_policy);
  // Return a new id-->object table.  If <user_id_policy> is true, the
  // request is being made for a POA with USER ID policy. Otherwise,
  // the SYSTEM_ID policy is being used.

  virtual ACE_Lock *create_poa_lock (void);
  // Return a new lock for use in locking the POA.

  virtual ACE_Lock *create_poa_mgr_lock (void);
  // Return a new lock for use in locking the POA Manager.

  virtual ACE_Lock *create_servant_lock (void);
  // Return a new lock for use in locking the servant.

  virtual ACE_Lock *create_event_loop_lock (void);
  // Return a new lock for use in the ORB event loop.

  virtual ACE_Lock *create_collocation_table_lock (void);
  // Create the lock used by the global collocation table.

  virtual ACE_Lock *create_cached_connector_lock (void);
  // Create the lock to be used by the cached connector.

  virtual u_long object_table_size (void) const;
  // Return the object table size
};

#endif /* TAO_SERVER_STRATEGY_FACTORY_H */
