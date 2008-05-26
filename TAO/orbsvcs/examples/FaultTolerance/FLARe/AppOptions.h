/**
 *  @file   AppOptions.h
 *
 *  @brief Declared the AppOptions class interface.
 *
 */

#ifndef _APPOPTIONS_H
#define _APPOPTIONS_H

#include <iostream>
#include <string>
#include "ace/Thread_Mutex.h"
#include "ArgPair.h"

/**
 *  @class   AppOptions
 *
 *  @brief Declares AppOptions singleton to hold the command line options.
 */

class AppOptions
/// TITLE
///   Singleton class for the program options.
{
public:

  /// Singleton access method.
  static AppOptions *instance (void);

  /// Parse command-line arguments and set the appropriate values as
  /// follows:
  bool parse_args (int argc, char **argv);
  std::string host_id () const;
  std::string host_monitor_ior () const;
  size_t get_port () const;
  std::string ior_output_file () const;
  std::string object_info_file () const;
  std::string process_id () const;
  ArgPair arg_pair () const;


protected:

  /// Constructor is protected to ensure Singleton access.
  AppOptions (void);

  std::string host_monitor_ior_;
  std::string host_id_;
  size_t port_;
  std::string ior_output_file_;
  std::string object_info_file_;
  std::string process_id_;
  ArgPair arg_pair_;

  /// Singleton instance.
  static AppOptions * volatile instance_;
  static std::auto_ptr<AppOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};


#endif /* _APPOPTIONS_H */
