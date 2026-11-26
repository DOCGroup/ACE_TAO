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
#include "UpdateableServerInfo.h"
#include <memory>
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

/// Gets a request from a client and depending on the POA name,
/// requests an activator to take care of activating the
/// corresponding server and raises a forward exception to the
/// client pointing to the correct server.
class Locator_Export ImR_Locator_i :
  public virtual POA_ImplementationRepository::AMH_Locator
{
public:
  ImR_Locator_i();

  ~ImR_Locator_i ();

  /// Initialize the service, creating its own orb, poa, etc.
  int init (Options& opts);

  /// Same as above, but use the given orb
  int init_with_orb (CORBA::ORB_ptr orb);

  /// Cleans up any state created by init*.
  int fini ();

  /// Run using the orb reference created during init()
  int run ();

  /// Called by the signal handler to notify of shutdown request
  void signal_shutdown ();

  /// Shutdown the orb.
  void shutdown (bool wait_for_completion);

  /// Access the options
  const Options *opts () const;

  static int debug ();
  /// See the IDL for descriptions of the operations.

  /// Activator->Locator

  void register_activator
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     char const *name,
     ImplementationRepository::Activator_ptr admin) override;

  void unregister_activator
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     char const *name,
     CORBA::Long token) override;

  void child_death_pid
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     char const *name, CORBA::Long pid) override;

  void spawn_pid
    (ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
     char const *name, CORBA::Long pid) override;

  // tao_imr->Locator

  void activate_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name) override;

  void add_or_update_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name,
     const ImplementationRepository::StartupOptions &options) override;

  void link_servers
    (ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
     char const *name,
     CORBA::StringSeq const & peers) override;

  void kill_server
    (ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
     char const *name,
     CORBA::Short signum) override;

  void remove_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name) override;

  void shutdown_server
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name) override;

  void force_remove_server
    (ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
     char const *name,
     CORBA::Short signum) override;

  void find
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name) override;

  void list
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     CORBA::ULong how_many,
     CORBA::Boolean determine_active_status) override;

  void shutdown
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     CORBA::Boolean activators,
     CORBA::Boolean servers) override;

  // Server->Locator

  void server_is_running
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name,
     char const *partial_ior,
     ImplementationRepository::ServerObject_ptr server_object) override;

  void server_is_shutting_down
    (ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
     char const *name) override;

  // Used by the INS_Locator to start a sever given an object name synchronously
  char* activate_server_by_object (char const *object_name);
  char* activate_server_by_name (char const *name, bool manual_start);

  // Asynchronous versions, used by both INS_Locator and Forwarder
  void activate_server_by_info (const Server_Info_Ptr &si,
                                ImR_ResponseHandler *rh);

  void activate_server_by_name (char const *name,
                                bool manual_start,
                                ImR_ResponseHandler *rh);

  bool split_key (ACE_CString &full, ACE_CString &key, Server_Info_Ptr &si);

  // interfaces to aid with collaboration

  LiveCheck &pinger ();
  PortableServer::POA_ptr root_poa ();
  Activator_Info_Ptr get_activator (ACE_CString const& name);

  void remove_server_i (const Server_Info_Ptr &si);
  void destroy_poa (ACE_CString const &poa_name);
  void remove_aam (AsyncAccessManager_ptr &aam);
  void remove_aam (char const *name);
  AsyncAccessManager *find_aam (char const *name, bool active = true);
  AsyncAccessManager *find_not_running_aam (char const *name, bool active = true);
  AsyncAccessManager *create_aam (UpdateableServerInfo &info, bool running = false);
  void make_terminating (AsyncAccessManager_ptr &aam, char const *name, int pid);
  /// Receiving an update from remote peer
  void remote_access_update (char const *name,
                             ImplementationRepository::AAM_Status state);

  CORBA::Object_ptr set_timeout_policy(CORBA::Object_ptr obj,
                                       ACE_Time_Value const& to);

private:
  bool get_info_for_name (char const *name, Server_Info_Ptr &si);

  void  activate_server_i (UpdateableServerInfo& info,
                           bool manual_start,
                           ImR_ResponseHandler *rh);

  bool server_is_alive (UpdateableServerInfo& info);

  void unregister_activator_i (char const *activator);

  void connect_activator (Activator_Info& info);

  void auto_start_servers ();

  void connect_server (UpdateableServerInfo& info);

  PortableServer::POA_ptr findPOA (char const *name);

  void child_death_i (char const *name, int pid);

  void remove_aam_i (char const *name, bool active);

  bool kill_server_i (const Server_Info_Ptr &si,
                      CORBA::Short signum,
                      CORBA::Exception *&ex);

  bool shutdown_server_i (const Server_Info_Ptr &si,
                          CORBA::Exception *&ex,
                          bool force);

private:
  static int debug_;

  /// The class that handles the forwarding.
  ImR_DSI_Forwarder dsi_forwarder_;

  /// Used for the forwarding of any type of POA.
  ImR_Adapter adapter_;

  /// The locator interface for the IORTable
  IORTable::Locator_var ins_locator_;

  /// The asynch server ping adapter
  LiveCheck pinger_;

  /// A collection of asynch activator instances
  using AAM_Set = ACE_Unbounded_Set<AsyncAccessManager_ptr>;
  AAM_Set aam_active_;
  AAM_Set aam_terminating_;

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  Repository_Ptr repository_;

  Options *opts_;

  TAO_SYNCH_MUTEX lock_;

  class Shutdown_Handler : public ACE_Event_Handler
  {
  public :
    ImR_Locator_i *owner_;
    Shutdown_Handler (ImR_Locator_i *owner) : owner_ (owner) {}
    int handle_exception (ACE_HANDLE) override;
  } shutdown_handler_;
};

class SyncListener : public LiveListener
{
 public:
  SyncListener (char const *server, CORBA::ORB_ptr orb, LiveCheck &pinger);

  virtual ~SyncListener ();

  bool is_alive ();

  bool status_changed (LiveStatus status) override;

 private:
  CORBA::ORB_var orb_;
  LiveCheck &pinger_;
  LiveStatus status_;
  bool got_it_;
  bool callback_;
};

/*
 * @class ImR_Loc_ResponseHandler
 *
 * @brief specialized reply handler for Locator interface calls which have a
 * void return.
 */
class ImR_SyncResponseHandler : public ImR_ResponseHandler
{
public:
  ImR_SyncResponseHandler (char const *key, CORBA::ORB_ptr orb);
  virtual ~ImR_SyncResponseHandler ();

  void send_ior (char const *pior) override;
  void send_exception (CORBA::Exception *ex) override;

  char *wait_for_result ();

private:
  CORBA::String_var result_;
  CORBA::Exception *excep_;
  ACE_CString key_;
  CORBA::ORB_var orb_;
};

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
      LOC_FORCE_REMOVE_SERVER,
      LOC_REMOVE_SERVER,
      LOC_SHUTDOWN_SERVER,
      LOC_SERVER_IS_RUNNING,
      LOC_SERVER_IS_SHUTTING_DOWN
    };

  ImR_Loc_ResponseHandler (Loc_Operation_Id opid,
                           ImplementationRepository::AMH_AdministrationResponseHandler_ptr rh);
  ImR_Loc_ResponseHandler (Loc_Operation_Id opid,
                           ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr rh);
  virtual ~ImR_Loc_ResponseHandler ();

  void send_ior (char const *pior) override;
  void send_exception (CORBA::Exception *ex) override;

private:
  void send_ior_ext (char const *pior);
  void send_exception_ext (CORBA::Exception *ex);

  Loc_Operation_Id op_id_;
  ImplementationRepository::AMH_AdministrationResponseHandler_var resp_;
  ImplementationRepository::AMH_AdministrationExtResponseHandler_var ext_;
};

#include /**/ "ace/post.h"
#endif /* IMR_LOCATOR_I_H */
