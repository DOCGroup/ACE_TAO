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

#if !defined (TAO_SERVER_STRATEGY_FACTORY_H)
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

  virtual TAO_Object_Table_Impl *create_object_table (void);
  // Return a new key-->object table

  virtual ACE_Lock *create_poa_lock (void);
  // Return a new lock for use in locking the POA.

  virtual ACE_Lock *create_poa_mgr_lock (void);
  // Return a new lock for use in locking the POA Manager.

  virtual ACE_Lock *create_servant_lock (void);
  // Return a new lock for use in locking the servant.

  virtual ACE_Lock *create_event_loop_lock (void);
  // Return a new lock for use in the ORB event loop.

  virtual ACE_Lock *create_coltbl_lock (void);
  // Create the lock used by the global collocation table.
};

#endif /* TAO_SERVER_STRATEGY_FACTORY_H */
