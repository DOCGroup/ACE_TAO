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
#define TAO_SERVER_STRATEGY_FACTORY_H

#include "ace/Service_Object.h"
#include "ace/Strategies_T.h"
#include "tao/Connect.h"
#include "tao/params.h"

class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;
class TAO_ORB_Core;

class TAO_Server_Strategy_Factory : public ACE_Service_Object
{
  // = TITLE
  //    Base class for the server's abstract factory that manufactures
  //    various strategies of special utility to it.  This simply
  //    serves as an interface to a subclass that REALLY gets
  //    specified and loaded by the Service Configurator.
public:

  struct Active_Object_Map_Creation_Parameters
  {
    Active_Object_Map_Creation_Parameters (void);
    // Constructor.

    CORBA::ULong active_object_map_size_;
    // Default size of object lookup table.

    TAO_Demux_Strategy object_lookup_strategy_for_user_id_policy_;
    // The type of lookup/demultiplexing strategy being used for user
    // id policy

    TAO_Demux_Strategy object_lookup_strategy_for_system_id_policy_;
    // The type of lookup/demultiplexing strategy being used for
    // system id policy

    TAO_Demux_Strategy reverse_object_lookup_strategy_for_unique_id_policy_;
    // The type of reverse lookup/demultiplexing strategy being used
    // for the UNIQUE_ID policy

    int use_active_hint_in_ids_;
    // Flag to indicate whether the active hint should be used with
    // the IOR.

    int allow_reactivation_of_system_ids_;
    // Flag to indicate whether reactivations of servants was required
    // (under the system id policy).  If not, certain resources may
    // not be required.

    CORBA::ULong poa_map_size_;

    TAO_Demux_Strategy poa_lookup_strategy_for_transient_id_policy_;

    TAO_Demux_Strategy poa_lookup_strategy_for_persistent_id_policy_;

    int use_active_hint_in_poa_names_;
  };

  // = Initialization and termination methods.
  TAO_Server_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Server_Strategy_Factory(void);
  // Destructor.

  virtual int open (TAO_ORB_Core* orb_core);
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

  virtual ACE_Lock *create_poa_lock (void);
  // Return a new lock for use in locking the POA.

  virtual ACE_Lock *create_servant_lock (void);
  // Return a new lock for use in locking the servant.

  virtual ACE_Lock *create_event_loop_lock (void);
  // Return a new lock for use in the ORB event loop.

  virtual ACE_Lock *create_collocation_table_lock (void);
  // Create the lock used by the global collocation table.

  virtual ACE_Lock *create_cached_connector_lock (void);
  // Create the lock to be used by the cached connector.

  virtual const Active_Object_Map_Creation_Parameters &active_object_map_creation_parameters (void) const;
  // Return the active object map creation parameters.

protected:

  Active_Object_Map_Creation_Parameters active_object_map_creation_parameters_;
  // Active object map creation parameters.
};

#endif /* TAO_SERVER_STRATEGY_FACTORY_H */
