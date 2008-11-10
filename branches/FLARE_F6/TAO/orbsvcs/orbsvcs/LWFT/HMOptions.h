// -*- C++ -*-
// $Id$

/**
 *  @file   HMOptions.h
 *
 *  @brief Declared the HMOptions class interface.
 *
 */

#ifndef _HMOPTIONS_H
#define _HMOPTIONS_H

#include <string>

#include "ace/Thread_Mutex.h"
#include "ace/Auto_Ptr.h"

#include "ArgPair.h"

#include "host_monitor_export.h"

/**
 *  @class   HMOptions
 *
 *  @brief Declares HMOptions singleton to hold the command line options.
 */

class HostMonitor_Export HMOptions
/// TITLE
///   Singleton class for the program options.
{
public:

  /// Singleton access method.
  static HMOptions *instance (void);

  /// Parse command-line arguments and set the appropriate values as
  /// follows:
  bool parse_args (int argc, char **argv);
  std::string RM_ior (void) const;
  std::string HM_ior_file (void) const;
  std::string host_id (void) const;
  int RM_update_freq (void) const;
  int load_monitor_freq (void) const;
  std::string util_file (void) const;
  ArgPair arg_pair (void); 
  std::pair <char, std::string> ior_access () const;


protected:

  /// Constructor is protected to ensure Singleton access.
  HMOptions (void);

  std::string RM_ior_;
  std::string host_id_;
  std::string HM_ior_file_;
  std::string util_file_;
  ArgPair arg_pair_; 
  size_t RM_update_freq_;
  size_t load_monitor_freq_;
  std::pair <char, std::string> ior_access_;

  /// Singleton instance.
  static HMOptions * volatile instance_;
  static ACE_Auto_Ptr<HMOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};


#endif /* _HMOPTIONS_H */
