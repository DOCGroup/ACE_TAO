// $Id$
/**
 * @file CIAO_ServerActivator_Impl.h
 * @author William R. Otte
 */

#ifndef CIAO_SERVERACTIVATOR_H_
#define CIAO_SERVERACTIVATOR_H_

#include "ace/Process_Manager.h"
#include "ace/Event_Handler.h"
#include "ace/Condition_T.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Refcounted_Auto_Ptr.h"
#include "ace/Unbounded_Set_Ex.h"
#include "ace/Synch_Traits.h"
#include "ace/Condition_T.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/orbconf.h"
#include "ciao/Client_init.h"
#include "ciao/ComponentServer/CIAO_CS_ClientS.h"
#include "ciao/ComponentServer/CIAO_ComponentServerC.h"
#include "ciao/ComponentServer/CIAO_CS_Client_svnt_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace Deployment
  {
    //namespace
    //{
      /**
       * @brief The signal handler class for the SIGCHLD handling to avoid
       * zombies
       */
      class Child_Handler : public virtual ACE_Event_Handler
      {
      public:
        virtual int handle_signal (int,
                                   siginfo_t *,
                                   ucontext_t *)
        {
          // @@ Note that this code is not portable to all OS platforms
          // since it uses print statements within signal handler context.
          ACE_exitcode status;
          // makes a claal to the underlying os system call
          // -1 to wait for any child process
          // and WNOHANG so that it retuurns immediately
          ACE_OS::waitpid (-1 ,&status, WNOHANG, 0);

          return 0;
        }
      };
      //    }


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
    class CIAO_CS_Client_svnt_Export CIAO_ServerActivator_i
      : public virtual POA_CIAO::Deployment::ServerActivator
    {
    public:
      // Constructor
      CIAO_ServerActivator_i (CORBA::ULong def_spawn_delay,
                              const char * default_cs_path,
                              const char * cs_args,
                              bool multithreaded,
                              CIAO::Deployment::ComponentInstallation_ptr ci,
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa_);

      // Destructor
      virtual ~CIAO_ServerActivator_i (void);

      virtual
      void component_server_callback (
        ::Components::Deployment::ComponentServer_ptr serverref,
        const char * server_UUID,
        ::Components::ConfigValues_out config);

      virtual void configuration_complete (const char *server_UUID);

      virtual
      ::Components::Deployment::ComponentServer_ptr
      create_component_server (const ::Components::ConfigValues & config);

      virtual
      void remove_component_server (
        ::Components::Deployment::ComponentServer_ptr server);

      virtual
      ::Components::Deployment::ComponentServers * get_component_servers (void);

    private:
      struct Server_Info;

      /// Builds command line options based on configuration information.
      /// May modify the uuid of the component server.
      ACE_CString construct_command_line (Server_Info &si);

      /// Spawns the component server process, but does not wait for it
      /// to call back.
      pid_t spawn_component_server (const Server_Info &si,
                                    const ACE_CString &cmd_line);

      /// This method is only applicable when our program is configured as
      /// singled threaded . Internally it uses a <perform_work> blocking
      /// call to wait for NA object to call back
      void single_threaded_wait_for_callback (const Server_Info &si,
                                              ACE_Time_Value &timeout);

      /// This method is only applicable when our program is configured as
      /// multiple threaded. Internally it waits on a conditional variable
      /// that could be modified by the callback servant which runs in
      /// another thread
      void multi_threaded_wait_for_callback (const Server_Info &si,
                                             ACE_Time_Value &timeout);

      void create_component_server_config_values (const Server_Info &info,
                                                  Components::ConfigValues_out &config);

      struct Server_Info
      {
        Server_Info (size_t cmap_size_hint = 128)
          : cmap_ (new CIAO::Utility::CONFIGVALUE_MAP (cmap_size_hint)),
            ref_ (Components::Deployment::ComponentServer::_nil ()),
            pid_ (ACE_INVALID_PID),
            activated_ (false) {}

        typedef ACE_Refcounted_Auto_Ptr <CIAO::Utility::CONFIGVALUE_MAP,
                                         ACE_Null_Mutex> CONFIGVALUE_MAP_PTR;

        ACE_CString uuid_;
        CONFIGVALUE_MAP_PTR cmap_;
        Components::Deployment::ComponentServer_var ref_;
        pid_t pid_;
        bool activated_;
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
      typedef ACE_Unbounded_Set_Ex <Safe_Server_Info, _server_info> SERVER_INFOS;

      /// Default args to pass to all componentservers.
      ACE_CString default_args_;

      SERVER_INFOS server_infos_;

      ACE_Process_Manager process_manager_;

      Child_Handler child_handler_;

      CORBA::ULong spawn_delay_;

      /////*******NEW
      bool multithreaded_;

      CORBA::ORB_var orb_;

      PortableServer::POA_var poa_;

      ACE_CString cs_path_;

      ACE_CString cs_args_;

      TAO_SYNCH_MUTEX mutex_;

      ACE_Condition<TAO_SYNCH_MUTEX> condition_;
      
      CIAO::Deployment::ComponentInstallation_var ci_;
    };


  }
}
#endif /* CIAO_SERVERACTIVATOR_H_ */
