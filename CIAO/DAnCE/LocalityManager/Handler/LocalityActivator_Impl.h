// $Id$
/**
 * @file LocalityActivator_Impl.h
 * @author William R. Otte
 */

#ifndef DAnCE_LocalityActivator_H_
#define DAnCE_LocalityActivator_H_

#include "ace/Process_Manager.h"
#include "ace/Event_Handler.h"
#include "ace/Condition_T.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Set_Ex.h"
#include "ace/Synch_Traits.h"
#include "ace/Condition_T.h"
#include "tao/PortableServer/PortableServer.h"
#include "DAnCE/DAnCE_LocalityManagerS.h"
#include "DAnCE/DAnCE_Utility.h"

#include "NodeApplication/NodeApplication_Export.h"

#include <set>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace DAnCE
{
  /**
   * @author William R. Otte <wotte@dre.vanderbilt.edu>
   * @brief Default server activator for CIAO component servers.
   *
   * Implements the default component server activation strategy
   * which is to spawn new processes.  This is not thread-safe,
   * nor is it intended to be.  Containers are reated serially,
   * so there will be only one actor *modifying* data at a particular
   * point in time.
   */
  class DAnCE_LocalityActivator_i
    : public virtual ::POA_DAnCE::LocalityManagerActivator
  {
  public:
    /// Constructor
    DAnCE_LocalityActivator_i (CORBA::ULong def_spawn_delay,
                               const char * default_cs_path,
                               const char * cs_args,
                               bool multithreaded,
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa_);

    /// Destructor
    virtual ~DAnCE_LocalityActivator_i (void);

    virtual
      void locality_manager_callback (::DAnCE::LocalityManager_ptr serverref,
                                      const char * server_UUID,
                                      ::Deployment::Properties_out config);

    virtual void configuration_complete (const char *server_UUID);

    ::DAnCE::LocalityManager_ptr
        create_locality_manager (const ::Deployment::Properties & config);

    void remove_locality_manager (
                                  ::DAnCE::LocalityManager_ptr server);

    ::DAnCE::LocalityManager * get_locality_managers (void);

  private:
    struct Server_Info;
    class Server_Child_Handler;

    /// Builds command line options based on configuration information.
    /// May modify the uuid of the component server.
    ACE_CString construct_command_line (Server_Info &si);

    /// Spawns the component server process, but does not wait for it
    /// to call back.
    pid_t spawn_locality_manager (Server_Child_Handler* exit_handler,
                                  const ACE_CString &cmd_line);

    /// This method is only applicable when our program is configured as
    /// singled threaded . Internally it uses a @c perform_work blocking
    /// call to wait for NA object to call back
    void single_threaded_wait_for_callback (const Server_Info &si,
                                            ACE_Time_Value &timeout);

    /// This method is only applicable when our program is configured as
    /// multiple threaded. Internally it waits on a conditional variable
    /// that could be modified by the callback servant which runs in
    /// another thread
    void multi_threaded_wait_for_callback (Server_Info &si,
                                           ACE_Time_Value &timeout);

    void create_properties (const Server_Info &info,
                            Deployment::Properties_out config);

    struct Server_Info
    {
      Server_Info (size_t cmap_size_hint = 128)
        : cmap_ (new DAnCE::Utility::PROPERTY_MAP (cmap_size_hint)),
          ref_ (DAnCE::LocalityManager::_nil ()),
          pid_ (ACE_INVALID_PID),
          activated_ (false),
          terminated_ (false),
          mutex_ (),
          condition_ (mutex_)
      {}

      typedef ACE_Refcounted_Auto_Ptr <DAnCE::Utility::PROPERTY_MAP,
                                       ACE_Null_Mutex> PROPERTY_MAP_PTR;

      ACE_CString uuid_;
      PROPERTY_MAP_PTR cmap_;
      DAnCE::LocalityManager_var ref_;
      pid_t pid_;
      bool activated_;
      bool terminated_;
      TAO_SYNCH_MUTEX mutex_;
      ACE_Condition<TAO_SYNCH_MUTEX> condition_;
    };

    typedef ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> Safe_Server_Info;

    struct _server_info
    {
      bool operator() (const Safe_Server_Info &a, const Safe_Server_Info &b) const
      {
        return a->uuid_ == b->uuid_;
      }
    };

    // Presumably, there won't be too many component servers per node application
    typedef std::set <Safe_Server_Info, _server_info> SERVER_INFOS;

    /**
    * @brief The exit handler class for the locality manager child process
    * to detect and report process exit
    */
    class Server_Child_Handler : public virtual ACE_Event_Handler
    {
    public:
      Server_Child_Handler (const Safe_Server_Info&  si);
      virtual ~Server_Child_Handler ();

      virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

      virtual int handle_exit (ACE_Process *proc);

      const Server_Info& server_info () const
      {
        return *this->server_info_;
      }
    private:
      Safe_Server_Info server_info_;
    };

    /// Default args to pass to all componentservers.
    ACE_CString default_args_;

    TAO_SYNCH_MUTEX container_mutex_;

    SERVER_INFOS server_infos_;

    ACE_Process_Manager process_manager_;

    CORBA::ULong spawn_delay_;

    /////*******NEW
    bool multithreaded_;

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    ACE_CString cs_path_;

    ACE_CString cs_args_;

    TAO_SYNCH_MUTEX mutex_;

    ACE_Condition<TAO_SYNCH_MUTEX> condition_;
  };


}
#endif /* DAnCE_LocalityActivator_H_ */
