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

#include "AppSideMonitor_Thread.h"
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

  ~AppOptions (void);

  bool parse_args (int &argc, char **argv);
  
  /// Member accessors.
  std::string host_id (void) const;
  std::string host_monitor_ior (void) const;
  u_short port (void) const;
  std::string object_info_file (void) const;
  std::string process_id (void) const;
  void monitor (AppSideMonitor_Thread *mon);
  std::string app_id (void) const;
  size_t role (void) const;
  double load (void) const;
  bool use_dds (void) const;

  /**
   *  @return the debug level of the application:
   *          0 - all errors and other urgent levels
   *          1 - warnings are shown as well
   *          2 - LM_DEBUG messages added
   *          3 - LM_TRACE messages added
   *          4 - LM_NOTICE messages added
   *          5 - LM_INFO messages added
   */
  long debug_level (void) const;

  void process_id (const std::string & id);

protected:
  /// Constructor is protected to ensure Singleton access.
  AppOptions (void);

  void set_debug_level (void);

  std::string host_monitor_ior_;
  std::string host_id_;
  u_short port_;
  std::string object_info_file_;
  std::string process_id_;
  std::string app_id_;
  size_t role_;
  double load_;
  bool use_dds_;
  long debug_level_;
  std::auto_ptr <AppSideMonitor_Thread> monitor_;

  /// Singleton-related stuff.
  static AppOptions * volatile instance_;
  static ACE_Auto_Ptr<AppOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};

#endif /* _APPOPTIONS_H */
