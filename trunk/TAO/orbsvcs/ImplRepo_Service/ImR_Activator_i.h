// -*- C++ -*-
//=============================================================================
/**
*  @file   ImR_Activator_i.h
*
*  $Id$
*
*  @author Priyanka Gontla <gontla_p@ociweb.com>
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#ifndef IMR_ACTIVATOR_I_H
#define IMR_ACTIVATOR_I_H

#include "activator_export.h"

#include "ImR_ActivatorS.h"
#include "ImR_LocatorC.h"

#include "ace/Process_Manager.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Activator_Options;

// ace/Functor.h doesn't provide functors for every built in integer type.
// Depending on the platform and what pid_t maps to, the functors may be missing.
struct ACE_Hash_pid_t
{
  unsigned long operator () (pid_t t) const
  {
    return t;
  }
};

struct ACE_Equal_To_pid_t
{
  int operator () (const pid_t lhs, const pid_t rhs) const
  {
    return lhs == rhs;
  }
};


/**
* @class ImR_Activator_i
*
* @brief IMR Activator Interface.
*
* This class provides the interface for the various activities
* that can be done by the ImR_Activator.
*
*/
class Activator_Export ImR_Activator_i : public POA_ImplementationRepository::ActivatorExt,
                                         public ACE_Event_Handler
{
 public:
  ImR_Activator_i (void);

  void start_server (const char* name,
                     const char* cmdline,
                     const char* dir,
                     const ImplementationRepository::EnvironmentList & env);

  CORBA::Boolean kill_server (const char* name, CORBA::Long pid, CORBA::Short signum);

  void shutdown(void);

  /// Initialize the Server state - parsing arguments and waiting.
  int init (Activator_Options& opts);

  /// Cleans up any state created by init*.
  int fini (void);

  int handle_timeout (const ACE_Time_Value &, const void *tok);

  /// Runs the orb.
  int run (void);

  /// Shutdown the orb.
  void shutdown (bool wait_for_completion);

private:

  int init_with_orb (CORBA::ORB_ptr orb, const Activator_Options& opts);

  void register_with_imr(ImplementationRepository::Activator_ptr activator);

  // Handles the death of the child processes of the ImR_Activator.
  // Informs the ImR_Locator too.
  int handle_exit (ACE_Process * process);
  int handle_exit_i (pid_t pid);

private:

  typedef ACE_Hash_Map_Manager_Ex<pid_t,
                                  ACE_CString,
                                  ACE_Hash_pid_t,
                                  ACE_Equal_To_pid_t,
                                  ACE_Null_Mutex> ProcessMap;

  ACE_Process_Manager process_mgr_;

  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  ImplementationRepository::Locator_var locator_;

  /// We're given a token when registering with the locator, which
  /// we must use when unregistering.
  CORBA::Long registration_token_;

  CORBA::ORB_var orb_;

  unsigned int debug_;

  bool notify_imr_;

  unsigned int induce_delay_;

  ACE_CString name_;

  ProcessMap process_map_;

  /// The default environment buffer length
  int env_buf_len_;

  /// Maximum number of environment variables
  int max_env_vars_;
};

#endif /* IMR_ACTIVATOR_I_H */
