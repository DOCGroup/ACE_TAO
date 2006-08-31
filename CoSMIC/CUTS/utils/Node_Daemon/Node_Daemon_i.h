/* -*- C++ -*- */

//=============================================================================
/**
 * @file      Node_Daemon_i.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_NODE_DAEMON_I_H_
#define _CUTS_NODE_DAEMON_I_H_

#include "cuts/config.h"
#include "Node_DaemonS.h"
#include "Node_Daemon_Event_Handler.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Process_Manager.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/SString.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue_Adapters.h"

namespace CUTS
{
  //===========================================================================
  /**
   * @class Node_Daemon_i
   *
   * Implementation of the CUTS::Node_Daemon interface.
   */
  //===========================================================================

  class Node_Daemon_i :
    public virtual POA_CUTS::Node_Daemon,
    public ACE_Event_Handler
  {
    // Friend decl.
    friend class Node_Daemon_Event_Handler;

  public:
    /// Default contructor.
    Node_Daemon_i (::CORBA::ORB_ptr orb);

    /// Destructor.
    virtual ~Node_Daemon_i (void);

    /**
     * Spawn a set of node daemons.
     *
     * @param[in]     nodes       Details of nodes to spawn.
     * @return        Number of nodes successfully spawned.
     */
    virtual ::CORBA::ULong spawn (
      const ::CUTS::Spawn_Detail & detail
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    /**
     * Kill a set of node daemons.
     *
     * @param[in]     nodes       Node bindings to kill.
     * @return        Number of nodes successfully killed.
     */
    virtual ::CORBA::ULong kill (
      const ::CUTS::Node_Bindings & nodes
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    /// Get the details of the spawned node managers.
    virtual ::CUTS::Node_Bindings * details (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    /// Shutdown the node daemon server.
    virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException));

    /**
     * Determine the availability of a specific binding.
     *
     * @param[in]       port          The port of interest.
     * @param[in]       localhost     The locality of the port.
     * @retval          true          The binding is available.
     * @retval          false         The binding is not available.
     */
    bool is_port_available (u_short port, bool localhost) const;

    /// Recover the node daemon to its previous state. This will
    /// cause to node daemon to manage any active processes it
    /// knew about before the failure.
    size_t recover (void);

    /// Clean the log file.
    void clean (void);

  private:
    /// Timeout handler for the cleaning thread.
    int handle_timeout (const ACE_Time_Value & tv,
                        const void * act);

    /// Initialize the class.
    void init (void);

    /// Initialize the \a p_options_ variable.
    void init_p_options (void);

    void bind_pid (pid_t pid,
                   u_short port,
                   bool localhost);

    /**
     * Helper method for spawning a node manager.
     *
     * @param[in]     port        Target port number.
     * @param[in]     localhost   Visibility of the port.
     * @param[in]     p_options   Process options for spawing.
     */
    pid_t spawn_i (u_short port,
                   bool localhost,
                   ACE_Process_Options & p_options);

    /**
     * Notify the node daemon that a particular process has
     * exited.
     *
     * @param[in]     pid         Id of the process.
     */
    void process_exits (pid_t pid);

    /// Type definition of mapping ports to processes.
    typedef ACE_Hash_Map_Manager <u_short,
                                  pid_t,
                                  ACE_Null_Mutex> Node_Detail_Map;

    /// Type definition for indexing the pid to it's entry.
    typedef ACE_Hash_Map_Manager <pid_t,
                                  Node_Detail_Map::ENTRY *,
                                  ACE_Null_Mutex> Process_Map;

    /// Mapping of process-ids to their node detail entry.
    Process_Map proc_map_;

    /// Collection of used local ports.
    Node_Detail_Map local_;

    /// Collection of used global ports.
    Node_Detail_Map global_;

    /// Process manager of the daemon used to spawn managers.
    ACE_Process_Manager pm_;

    /// IP-address of the node daemon.
    ACE_CString ip_addr_;

    /// Full path name of the node manager.
    ACE_CString node_manager_;

    /// Event handler for the node daemon.
    Node_Daemon_Event_Handler event_handler_;

    /// Common process options used by the daemon.
    ACE_Process_Options p_options_;

    /// The timer queue for the periodic task.
    ACE_Thread_Timer_Queue_Adapter <ACE_Timer_Heap> timer_queue_;

    /// Timeout value for the cleaning thread.
    long timer_;

    /// Locking mechanism for the mappings.
    ACE_RW_Thread_Mutex lock_;

    /// The ORB hosting the node daemon.
    ::CORBA::ORB_var orb_;
  };
}

#if defined (__CUTS_INLINE__)
#include "Node_Daemon_i.inl"
#endif

#endif  /* !defined _CUTS_NODE_DAEMON_I_H_ */
