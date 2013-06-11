// -*- C++ -*-
/*
 * @file LiveCheck.h
 *
 * $Id$
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 */

#ifndef IMR_LIVECHECK_H_
#define IMR_LIVECHECK_H_

#include "locator_export.h"

#include "ServerObjectS.h" // ServerObject_AMIS.h

#include "ace/Unbounded_Set.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Intrusive_Ref_Count_Handle_T.h"

class LiveCheck;
class LiveEntry;

//---------------------------------------------------------------------------
/*
 * @enum LiveStatus
 *
 * @brief indication of the known condition of a target server
 *
 *  LS_UNKNOWN   - The server hasn't yet been pinged
 *  LS_PING_AWAY - A ping request has been issued, waiting for reply
 *  LS_DEAD      - The ping failed for reasons other than POA Activation
 *  LS_ALIVE     - The server positively acknowledged a ping
 *  LS_TRANSIENT - The server connected, but acively raised a transient
 *  LS_LAST_TRANSIENT - The maximum number of retries is reached
 *  LS_TIMEDOUT  - The server connected, but never returned any result.
 */
enum LiveStatus {
  LS_UNKNOWN,
  LS_PING_AWAY,
  LS_DEAD,
  LS_ALIVE,
  LS_TRANSIENT,
  LS_LAST_TRANSIENT,
  LS_TIMEDOUT
};

//---------------------------------------------------------------------------
/*
 * @class LiveListener
 *
 * @brief An interface for receiving asynch liveness status updates
 *
 * The code waiting on a confirmation of liveness status creates an instance
 * of a LiveListener and registers it with the LiveCheck object.
 * When the desired ping occurs, the status_changed method is called and the
 * listener is unregistered. It is up to the owner of the listener to re-
 * register if the ping result was inconclusive, such as a status of TRANSIENT
 * or TIMEDOUT. Such a decision is based on configuration settings.
 */
class Locator_Export LiveListener
{
 public:
  /// Construct a new listener. The server name suppled is used to
  /// look up a listener entry in the LiveCheck map.
  LiveListener (const char *server);

  virtual ~LiveListener (void);

  /// sets the initial status volue, set by entry during an add_listener call
  void entry (const LiveEntry *entry);

  /// Query the status of the associated entry
  LiveStatus status (void) const;

  /// called by the asynch ping receiver when a reply or an exception
  /// is received. Returns true if finished listening
  virtual bool status_changed (LiveStatus status) = 0;

  /// accessor for the server name. Used by the LiveCheck to associate a listener
  const char *server (void) const;

  LiveListener *_add_ref (void);
  void _remove_ref (void);

 protected:
  ACE_CString server_;
  const LiveEntry *entry_;

 private:
  int refcount_;
  TAO_SYNCH_MUTEX lock_;
};

typedef TAO_Intrusive_Ref_Count_Handle<LiveListener> LiveListener_ptr;

//---------------------------------------------------------------------------
/*
 * @class LiveEntry
 *
 * @brief Contains a list of interested listeners for a server
 *
 * Each server the Locator is interested in has a live entry instance.
 * This holds the liveliness status and determines the next allowed time
 * for a ping. Instances of the LiveEntry class are retained until the
 * locator is no longer interested in the target server.
 */
class Locator_Export LiveEntry
{
 public:
  LiveEntry (LiveCheck *owner,
             const char *server,
             bool may_ping,
             ImplementationRepository::ServerObject_ptr ref);
  ~LiveEntry (void);

  void add_listener (LiveListener *ll);
  LiveStatus status (void) const;
  void status (LiveStatus l);
  void reset_status (void);

  /// the current state value as text
  static const char *status_name (LiveStatus s);

  void update_listeners (void);
  bool validate_ping (bool &want_reping, ACE_Time_Value &next);
  void do_ping (PortableServer::POA_ptr poa);
  const ACE_Time_Value &next_check (void) const;
  static void set_reping_limit (int max);
  bool reping_available (void);
  int next_reping (void);
  void max_retry_msec (int max);

 private:
  LiveCheck *owner_;
  ACE_CString server_;
  ImplementationRepository::ServerObject_var ref_;
  LiveStatus liveliness_;
  ACE_Time_Value next_check_;
  int repings_;
  int max_retry_;
  bool may_ping_;

  typedef ACE_Unbounded_Set<LiveListener_ptr> Listen_Set;
  Listen_Set listeners_;
  TAO_SYNCH_MUTEX lock_;
  static const int reping_msec_ [];
  static int reping_limit_;

};

//---------------------------------------------------------------------------
/*
 * @class PingReceiver
 *
 * @brief callback handler for asynch ping requests
 *
 * An instance of the ping receiver is used to handle the reply from a ping
 * request. Instances are created for the ping, then destroyed.
`*/
class Locator_Export PingReceiver :
  public virtual POA_ImplementationRepository::AMI_ServerObjectHandler
{
 public:
  PingReceiver (LiveEntry * entry, PortableServer::POA_ptr poa);
  virtual ~PingReceiver (void);

  void ping (void);
  void ping_excep (Messaging::ExceptionHolder * excep_holder);

 private:
  PortableServer::POA_var poa_;
  LiveEntry * entry_;
};

//---------------------------------------------------------------------------
/*
 * @class LiveCheck
 *
 * @brief The manager class used for pinging servers as needed.
 *
 * The LiveCheck class maintains a map of named LiveEntries. When the locator
 * needs to determine the liveliness of a server, registers a LiveListener
 * for the desired server. A ping to the server is then scheduled, based on the
 * limits determined by the entry's state.
 */
class Locator_Export LiveCheck : public ACE_Event_Handler
{
 public:
  LiveCheck ();
  ~LiveCheck (void);

  void init (CORBA::ORB_ptr orb,
             const ACE_Time_Value &interval);
  void shutdown (void);

  int handle_timeout (const ACE_Time_Value &current_time,
                      const void *act = 0);

  bool has_server (const char *server);

  void add_server (const char *server,
                   bool may_ping,
                   ImplementationRepository::ServerObject_ptr ref);

  void remove_server (const char *server);

  void remove_per_client_entry (LiveEntry *entry);

  bool add_listener (LiveListener *listener);

  bool add_poll_listener (LiveListener *listener);

  bool add_per_client_listener (LiveListener *listener,
                                ImplementationRepository::ServerObject_ptr ref);

  bool schedule_ping (LiveEntry *entry);

  LiveStatus is_alive (const char *server);

  const ACE_Time_Value &ping_interval (void) const;

 private:
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  LiveEntry *,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  TAO_SYNCH_MUTEX> LiveEntryMap;
  typedef ACE_Unbounded_Set<LiveEntry *> PerClientStack;

  LiveEntryMap entry_map_;
  PerClientStack per_client_;
  PortableServer::POA_var poa_;
  ACE_Time_Value ping_interval_;
  bool running_;
  int token_;
};

#endif /* IMR_LIVECHECK_H_  */

