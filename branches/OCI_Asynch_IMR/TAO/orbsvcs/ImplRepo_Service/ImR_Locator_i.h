// -*- C++ -*-
// $Id$

#ifndef IMR_LOCATOR_I_H
#define IMR_LOCATOR_I_H
#include /**/ "ace/pre.h"

#include "locator_export.h"

#include "Adapter_Activator.h"
#include "Activator_Info.h"
#include "Forwarder.h"
#include "LiveCheck.h"
#include "Locator_Options.h"
#include "Server_Info.h"
#include "ace/Auto_Ptr.h"
#include "AsyncStartupWaiter_i.h"
#include "tao/IORTable/IORTable.h"

#include "ImR_LocatorS.h"
#include "AsyncStartupWaiterS.h"
#include "LiveCheck.h"
//#include "AsyncAccessManager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

class INS_Locator;
class Locator_Repository;
class UpdateableServerInfo;

/// Gets a request from a client and depending on the POA name,
/// requests an activator to take care of activating the
/// corresponding server and raises a forward exception to the
/// client pointing to the correct server.
class Locator_Export ImR_Locator_i : public virtual POA_ImplementationRepository::Locator
{
public:
  ImR_Locator_i();

  ~ImR_Locator_i (void);

  /// Initialize the service, creating its own orb, poa, etc.
  int init (Options& opts);

  /// Same as above, but use the given orb
  int init_with_orb (CORBA::ORB_ptr orb, Options& opts);

  /// Cleans up any state created by init*.
  int fini (void);

  /// Run using the orb reference created during init()
  int run (void);

  /// Shutdown the orb.
  void shutdown (bool wait_for_completion);

  int debug() const;
  // Note : See the IDL for descriptions of the operations.

  // Activator->Locator

  virtual CORBA::Long register_activator (const char* name,
    ImplementationRepository::Activator_ptr admin);
  virtual void unregister_activator (const char* name,
    CORBA::Long token);
  virtual void notify_child_death (const char* name);

  // tao_imr->Locator

  virtual void activate_server (const char * name);
  virtual void add_or_update_server (const char * name,
               const ImplementationRepository::StartupOptions &options);
  virtual void remove_server (const char * name);
  virtual void shutdown_server (const char * name);
  virtual void find (const char * name,
                     ImplementationRepository::ServerInformation_out info);
  virtual void list (CORBA::ULong how_many,
    CORBA::Boolean determine_active_status,
    ImplementationRepository::ServerInformationList_out server_list,
    ImplementationRepository::ServerInformationIterator_out server_iterator);
  virtual void shutdown(CORBA::Boolean activators, CORBA::Boolean servers);

  // Server->Locator

  virtual void server_is_running (const char* name,
                                  const char* partial_ior,
               ImplementationRepository::ServerObject_ptr server_object);
  virtual void server_is_shutting_down (const char * name);

  // Used by the INS_Locator to start a sever given an object name
  char* activate_server_by_object (const char* object_name);
  char* activate_server_by_name (const char * name, bool manual_start);
  void  activate_server_by_name (const char * name,
                                 bool manual_start,
                                 ImR_ReplyHandler *rh);

private:

  char* activate_server_i (UpdateableServerInfo& info,
                           bool manual_start);

  void  activate_server_i (UpdateableServerInfo& info,
                           bool manual_start,
                           ImR_ReplyHandler *rh);

  char* activate_perclient_server_i (UpdateableServerInfo& info,
                                     bool manual_start);

  void  activate_perclient_server_i (UpdateableServerInfo& info,
                                     bool manual_start,
                                     ImR_ReplyHandler *rh);

  ImplementationRepository::StartupInfo*
    start_server(UpdateableServerInfo& info,
                 bool manual_start,
                 int& waiting_clients);

  bool is_alive(UpdateableServerInfo& info);
  int is_alive_i(UpdateableServerInfo& info);

  void unregister_activator_i(const char* activator);

  Activator_Info_Ptr get_activator (const ACE_CString& name);
  void connect_activator (Activator_Info& info);

  void auto_start_servers(void);

  CORBA::Object_ptr set_timeout_policy(CORBA::Object_ptr obj,
                                       const ACE_Time_Value& to);

  void connect_server(UpdateableServerInfo& info);

  PortableServer::POA_ptr findPOA(const char* name);

  void parse_id(const char* id, ACE_CString& server_id, ACE_CString& name, bool& jacorb_server);
private:

  // The class that handles the forwarding.
  ImR_Forwarder forwarder_;

  // The alternative forwarder uses DSI/AMH for asynch forwarding.
  ImR_DSI_Forwarder dsi_forwarder_;

  // Used for the forwarding of any type of POA.
  ImR_Adapter adapter_;

  /// The locator interface for the IORTable
  IORTable::Locator_var ins_locator_;

  /// The asynch server ping adapter
  LiveCheck pinger_;

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  int debug_;

  auto_ptr<Locator_Repository> repository_;

  AsyncStartupWaiter_i waiter_svt_;
  ImplementationRepository::AsyncStartupWaiter_var waiter_;

  bool read_only_;
  ACE_Time_Value startup_timeout_;
  ACE_Time_Value ping_interval_;
  bool unregister_if_address_reused_;
  bool use_asynch_;
};

//----------------------------------------------------------------------------
/*
 */

class SyncListener : public LiveListener
{
 public:
  SyncListener (const char *server, CORBA::ORB_ptr orb, LiveCheck &pinger);

  bool is_alive (void);

  void status_changed (LiveStatus status);

 private:
  CORBA::ORB_var orb_;
  LiveCheck &pinger_;
  LiveStatus status_;
  bool got_it_;
  int retries_;
};


#include /**/ "ace/post.h"
#endif /* IMR_LOCATOR_I_H */
