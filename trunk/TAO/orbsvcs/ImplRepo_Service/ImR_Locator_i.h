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
#include "ImR_ResponseHandler.h"
#include "Locator_Options.h"
#include "Server_Info.h"
#include "ace/Auto_Ptr.h"
#include "AsyncAccessManager.h"
#include "tao/IORTable/IORTable.h"

#include "ImR_LocatorS.h"

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
class Locator_Export ImR_Locator_i :
  public virtual POA_ImplementationRepository::AMH_Locator
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

  static int debug (void);
  // Note : See the IDL for descriptions of the operations.

  // Activator->Locator

  virtual void register_activator
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     const char* name,
     ImplementationRepository::Activator_ptr admin);

  virtual void unregister_activator
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     const char* name,
     CORBA::Long token);

  virtual void notify_child_death
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     const char* name);

  virtual void child_death_pid
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     const char* name, CORBA::Long pid);

  virtual void spawn_pid
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     const char* name, CORBA::Long pid);

  // tao_imr->Locator

  virtual void activate_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name);

  virtual void add_or_update_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name,
     const ImplementationRepository::StartupOptions &options);

  virtual void link_servers
    (ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
     const char * name,
     const CORBA::StringSeq & peers);

  virtual void kill_server
    (ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
     const char * name,
     CORBA::Short signum);

  virtual void remove_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name);

  virtual void shutdown_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name);

  virtual void find
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name);

  virtual void list
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     CORBA::ULong how_many,
     CORBA::Boolean determine_active_status);

  virtual void shutdown
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     CORBA::Boolean activators,
     CORBA::Boolean servers);

  // Server->Locator

  virtual void server_is_running
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char* name,
     const char* partial_ior,
     ImplementationRepository::ServerObject_ptr server_object);

  virtual void server_is_shutting_down
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     const char * name);

  // Used by the INS_Locator to start a sever given an object name synchronously
  char* activate_server_by_object (const char* object_name);
  char* activate_server_by_name (const char * name, bool manual_start);

  // Asynchronous versions, used by both INS_Locator and Forwarder
  void activate_server_by_info (const Server_Info_Ptr &si,
                                ImR_ResponseHandler *rh);

  void activate_server_by_name (const char * name,
                                bool manual_start,
                                ImR_ResponseHandler *rh);

  bool split_key (ACE_CString &full, ACE_CString &key, Server_Info_Ptr &si);

  // interfaces to aid with collaboration

  LiveCheck &pinger (void);
  PortableServer::POA_ptr root_poa (void);
  Activator_Info_Ptr get_activator (const ACE_CString& name);

  void remove_aam (AsyncAccessManager_ptr &aam);
  void remove_aam (const char *name);
  AsyncAccessManager *find_aam (const char *name);

private:

  bool get_info_for_name (const char *name, Server_Info_Ptr &si);

  void  activate_server_i (UpdateableServerInfo& info,
                           bool manual_start,
                           ImR_ResponseHandler *rh);

  bool is_alive(UpdateableServerInfo& info);

  void unregister_activator_i(const char* activator);

  void connect_activator (Activator_Info& info);

  void auto_start_servers(void);

  CORBA::Object_ptr set_timeout_policy(CORBA::Object_ptr obj,
                                       const ACE_Time_Value& to);

  void connect_server(UpdateableServerInfo& info);

  PortableServer::POA_ptr findPOA(const char* name);

private:

  static int debug_;

  // The class that handles the forwarding.
  ImR_DSI_Forwarder dsi_forwarder_;

  // Used for the forwarding of any type of POA.
  ImR_Adapter adapter_;

  /// The locator interface for the IORTable
  IORTable::Locator_var ins_locator_;

  /// The asynch server ping adapter
  LiveCheck pinger_;

  /// A collection of asynch activator instances
  typedef ACE_Unbounded_Set<AsyncAccessManager_ptr> AAM_Set;
  AAM_Set aam_set_;

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  auto_ptr<Locator_Repository> repository_;

  bool read_only_;
  ACE_Time_Value startup_timeout_;
  bool ping_external_;
  ACE_Time_Value ping_interval_;
  bool unregister_if_address_reused_;
  bool throw_shutdown_exceptions_;
};

//----------------------------------------------------------------------------
/*
 */

class SyncListener : public LiveListener
{
 public:
  SyncListener (const char *server, CORBA::ORB_ptr orb, LiveCheck &pinger);

  virtual ~SyncListener ();

  bool is_alive (void);

  bool status_changed (LiveStatus status);

 private:
  CORBA::ORB_var orb_;
  LiveCheck &pinger_;
  LiveStatus status_;
  bool got_it_;
  bool callback_;
};

//----------------------------------------------------------------------------
/*
 * @class ImR_Loc_ResponseHandler
 *
 * @brief specialized reply handler for Locator interface calls which have a
 * void return.
 */
class ImR_SyncResponseHandler : public ImR_ResponseHandler
{
public:
  ImR_SyncResponseHandler (const char *key, CORBA::ORB_ptr orb);
  virtual ~ImR_SyncResponseHandler (void);

  virtual void send_ior (const char *pior);
  virtual void send_exception (CORBA::Exception *ex);

  char *wait_for_result (void);

private:
  CORBA::String_var result_;
  CORBA::Exception *excep_;
  ACE_CString key_;
  CORBA::ORB_var orb_;
};

//----------------------------------------------------------------------------
/*
 * @class ImR_Loc_ResponseHandler
 *
 * @brief specialized reply handler for Locator interface calls which have a
 * void return.
 */
class ImR_Loc_ResponseHandler : public ImR_ResponseHandler
{
public:
  enum Loc_Operation_Id
    {
      LOC_ACTIVATE_SERVER,
      LOC_ADD_OR_UPDATE_SERVER,
      LOC_REMOVE_SERVER,
      LOC_SHUTDOWN_SERVER,
      LOC_SERVER_IS_RUNNING,
      LOC_SERVER_IS_SHUTTING_DOWN
    };

  ImR_Loc_ResponseHandler (Loc_Operation_Id opid,
                           ImplementationRepository::AMH_AdministrationResponseHandler_ptr rh);
  virtual ~ImR_Loc_ResponseHandler (void);

  virtual void send_ior (const char *pior);
  virtual void send_exception (CORBA::Exception *ex);

private:
  Loc_Operation_Id op_id_;
  ImplementationRepository::AMH_AdministrationResponseHandler_var resp_;

};

#include /**/ "ace/post.h"
#endif /* IMR_LOCATOR_I_H */
