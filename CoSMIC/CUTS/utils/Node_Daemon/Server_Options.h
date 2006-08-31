/* -*- C++ -*- */

//=============================================================================
/**
 * @file        Server_Options.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _SERVER_OPTIONS_H_
#define _SERVER_OPTIONS_H_

#include "ace/Null_Mutex.h"
#include "ace/Singleton.h"
#include "ace/SString.h"

//=============================================================================
/**
 * @class Server_Options
 *
 * Collection of server options that can be set via command-line
 * arguments.
 */
//=============================================================================

class Server_Options
{
public:
  /// Default constructor.
  Server_Options (void);

  /// Destructor.
  ~Server_Options (void);

  /**
   * Set the path to the node manager. This method makes sure
   * the path ends in a slash.
   *
   * @param[in]         path          Path to store.
   */
  void node_manager_path (const char * path);

  /**
   * Get the path to the node manager.
   *
   * @return  NULL-terminated string.
   */
  const ACE_CString & node_manager_path (void) const;

  /// Flag specifying the verbosity of the server
  bool verbose_;

  /// The IOR file name for the daemon.
  ACE_CString ior_file_;

  /// The initial directory for starting the node daemon.
  ACE_CString init_dir_;

private:
  /// The path of the node daemon.
  ACE_CString path_;
};

#define SERVER_OPTIONS() \
  ACE_Singleton <Server_Options, ACE_Null_Mutex>::instance ()

#define VERBOSE_MESSAGE(msg) \
  if (SERVER_OPTIONS()->verbose_) \
  { \
    ACE_ERROR (msg); \
  }

#define VERBOSE_MESSAGE_RETURN(msg, retval) \
  if (SERVER_OPTIONS()->verbose_) \
  { \
    ACE_ERROR_RETURN (msg, retval); \
  }

#endif  /* !defined _SERVER_OPTIONS_H_ */
