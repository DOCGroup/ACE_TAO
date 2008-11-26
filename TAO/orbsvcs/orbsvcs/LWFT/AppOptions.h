// -*- C++ -*-
// $Id$

/**
 *  @file   AppOptions.h
 *
 *  @brief Declared the AppOptions class interface.
 *
 */

#ifndef _APPOPTIONS_H
#define _APPOPTIONS_H

#include <string>

#include "ace/Thread_Mutex.h"
#include "ace/Auto_Ptr.h"

#include "lwft_server_export.h"

/**
 *  @class   AppOptions
 *
 *  @brief Declares AppOptions singleton to hold the command line options.
 */

class LWFT_Server_Export AppOptions
/// TITLE
///   Singleton class for the program options.
{
public:
  /// Singleton access method.
  static AppOptions *instance (void);

  bool parse_args (int &argc, char **argv);
  
  /// Member accessors.
  std::string host_id (void) const;
  std::string host_monitor_ior (void) const;
  u_short port (void) const;
  std::string object_info_file (void) const;
  std::string process_id (void) const;
  std::string app_id (void) const;
  size_t role (void) const;
  double load (void) const;
  bool use_dds (void) const;

protected:
  /// Constructor is protected to ensure Singleton access.
  AppOptions (void);

  std::string host_monitor_ior_;
  std::string host_id_;
  u_short port_;
  std::string object_info_file_;
  std::string process_id_;
  std::string app_id_;
  size_t role_;
  double load_;
  bool use_dds_;

  /// Singleton-related stuff.
  static AppOptions * volatile instance_;
  static ACE_Auto_Ptr<AppOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};

#endif /* _APPOPTIONS_H */
