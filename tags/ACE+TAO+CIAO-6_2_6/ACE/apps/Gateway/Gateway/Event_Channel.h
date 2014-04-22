/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Channel.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_EVENT_CHANNEL
#define ACE_EVENT_CHANNEL

#include "Connection_Handler_Connector.h"
#include "Connection_Handler_Acceptor.h"
#include "Consumer_Dispatch_Set.h"
#include "Event_Forwarding_Discriminator.h"
#include "ace/svc_export.h"

typedef ACE_Null_Mutex MAP_MUTEX;

/**
 * @class Event_Channel
 *
 * @brief Define a generic Event_Channel.
 *
 * The inspiration for this class is derived from the CORBA COS
 * Event Channel, though the design is simplified.
 * We inherit from <ACE_Event_Handler> so that we can be
 * registered with an <ACE_Reactor> to handle timeouts.
 */
class ACE_Svc_Export Event_Channel : public ACE_Event_Handler
{
public:
  // = Initialization and termination methods.
  Event_Channel (void);
  ~Event_Channel (void);

  /// Open the channel.
  virtual int open (void * = 0);

  /// Close down the Channel.
  virtual int close (u_long = 0);

  // = Proxy management methods.
  /**
   * Initiate the connection of the <Connection_Handler> to its peer.
   * Second paratemer is used for thread connection-handler which will
   * block the connecting procedure directly, need not care
   * Options::blocking_semantics().
   */
  int initiate_connection_connection (Connection_Handler *, int sync_directly = 0);

  /// Complete the initialization of the <Connection_Handler> once it's
  /// connected to its Peer.
  int complete_connection_connection (Connection_Handler *);

  /// Reinitiate a connection asynchronously when the Peer fails.
  /// Cancel a asynchronous connection.
  int reinitiate_connection_connection (Connection_Handler *);
  int cancel_connection_connection (Connection_Handler *);

  /// Bind the <Connection_Handler> to the <connection_map_>.
  int bind_proxy (Connection_Handler *);

  /// Locate the <Connection_Handler> with <connection_id>.
  int find_proxy (ACE_INT32 connection_id,
                  Connection_Handler *&);

  /// Subscribe the <Consumer_Dispatch_Set> to receive events that
  /// match <Event_Key>.
  int subscribe (const Event_Key &event_addr,
                 Consumer_Dispatch_Set *cds);

  // = Event processing entry point.
  /// Pass <mb> to the Event Channel so it can forward it to Consumers.
  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value * = 0);

  /// Actively initiate connections to the Peers.
  void initiate_connector (void);

  /// Passively initiate the <Peer_Acceptor>s for Consumer and
  /// Suppliers.
  int initiate_acceptors (void);

private:
  /// Parse the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  // = Methods for handling events.
  /// Forwards the <data> to Consumer that have registered to receive
  /// it, based on addressing information in the <event_key>.
  void routing_event (Event_Key *event_key,
                    ACE_Message_Block *data);

  /// Add a Consumer subscription.
  void subscription_event (ACE_Message_Block *data);

  /// Perform timer-based performance profiling.
  int compute_performance_statistics (void);

  /// Periodically callback to perform timer-based performance
  /// profiling.
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg);

  /// Used to establish the connections actively.
  Connection_Handler_Connector connector_;

  /// Used to establish connections passively and create Suppliers.
  Connection_Handler_Acceptor supplier_acceptor_;

  /// Used to establish connections passively and create Consumers.
  Connection_Handler_Acceptor consumer_acceptor_;

  // = Make life easier by defining typedefs.
  typedef ACE_Map_Manager<CONNECTION_ID, Connection_Handler *, MAP_MUTEX>
  CONNECTION_MAP;
  typedef ACE_Map_Iterator<CONNECTION_ID, Connection_Handler *, MAP_MUTEX>
  CONNECTION_MAP_ITERATOR;
  typedef ACE_Map_Entry<CONNECTION_ID, Connection_Handler *>
  CONNECTION_MAP_ENTRY;

  /// Table that maps <CONNECTION_ID>s to <Connection_Handler> *'s.
  CONNECTION_MAP connection_map_;

  /// Map that associates an event to a set of <Consumer_Handler> *'s.
  Event_Forwarding_Discriminator efd_;
};

#endif /* ACE_EVENT_CHANNEL */
