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

#include "ArgPair.h"

/**
 *  @class   AppOptions
 *
 *  @brief Declares AppOptions singleton to hold the command line options.
 */

class TAO_LWFT_Export AppOptions
/// TITLE
///   Singleton class for the program options.
{
public:

  /// Singleton access method.
  static AppOptions *instance (void);

  /// Parse command-line arguments and set the appropriate values as
  /// follows:
  bool parse_args (int argc, char **argv);
  std::string host_id (void) const;
  std::string host_monitor_ior (void) const;
  u_short get_port (void) const;
  std::string ior_output_file (void) const;
  std::string object_info_file (void) const;
  std::string process_id (void) const;
  ArgPair arg_pair (void) const;


protected:

  /// Constructor is protected to ensure Singleton access.
  AppOptions (void);

  std::string host_monitor_ior_;
  std::string host_id_;
  u_short port_;
  std::string ior_output_file_;
  std::string object_info_file_;
  std::string process_id_;
  ArgPair arg_pair_;

  /// Singleton instance.
  static AppOptions * volatile instance_;
  static ACE_Auto_Ptr<AppOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};


#endif /* _APPOPTIONS_H */
