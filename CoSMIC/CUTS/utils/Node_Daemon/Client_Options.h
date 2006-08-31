// -*- C++ -*-

//=============================================================================
/**
 * @file      Client_Options.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CLIENT_OPTIONS_H_
#define _CLIENT_OPTIONS_H_

#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

//=============================================================================
/**
 * @class Client_Options
 *
 * Collection of configurable client options.
 */
//=============================================================================

class Client_Options
{
public:
  /// Default constructor.
  Client_Options (void);

  /// Verbose setting for the client.
  bool verbose_;

  /// Visibility of the node daemons processes.
  bool localhost_;

  /// Number of managers to processes.
  size_t count_;

  /// Type of actions the client can perform.
  enum Client_Action
  {
    /// Perform no action.
    CA_NONE   = 0,

    /// Spawn node managers.
    CA_SPAWN  = 1,

    /// Kill node managers.
    CA_KILL   = 2
  };

  /// Type of action to perform.
  Client_Action action_;

  /// Signal the node daemon to shutdown.
  bool shutdown_;

  /// Default port number.
  u_short port_;

  /// Arguments used for spawning.
  ACE_CString args_;

  /// Get details about the node.
  bool details_;
};

/// Singleton for the client options.
#define CLIENT_OPTIONS() \
  ACE_Singleton <Client_Options, ACE_Null_Mutex>::instance ()

/// Wrapper for writing a ACE_ERROR based on the verbose flag
/// in Client_Options.
#define VERBOSE_MESSAGE(msg) \
  if (CLIENT_OPTIONS()->verbose_) \
  { \
    ACE_ERROR (msg); \
  }

/// Wrapper for writing a ACE_ERROR_RETURN based on the verbose
/// flag in Client_Options.
#define VERBOSE_MESSAGE_RETURN(msg, retval) \
  if (CLIENT_OPTIONS()->verbose_) \
  { \
    ACE_ERROR_RETURN (msg, retval); \
  }


#endif  // !defined _CLIENT_OPTIONS_H_
