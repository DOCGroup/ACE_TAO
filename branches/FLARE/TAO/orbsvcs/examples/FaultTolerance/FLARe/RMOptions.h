/**
 *  @file   RMOptions.h
 *
 *  @brief Declared the RMOptions class interface.
 *
 */

#ifndef _HMOPTIONS_H
#define _HMOPTIONS_H

#include <iostream>
#include <string>
#include "ace/Thread_Mutex.h"


/**
 *  @class   RMOptions
 *
 *  @brief Declares RMOptions singleton to hold the command line options.
 */

#include "ArgPair.h"

class RMOptions
/// TITLE
///   Singleton class for the program options.
{
public:

  /// Singleton access method.
  static RMOptions *instance (void);

  /// Parse command-line arguments and set the appropriate values as
  /// follows:
  bool parse_args (int argc, char **argv);
  bool proactive () const;
  double hertz() const;

protected:

  /// Constructor is protected to ensure Singleton access.
  RMOptions (void);

  double hertz_;
  bool proactive_;
  ArgPair arg_pair_;

  /// Singleton instance.
  static RMOptions * volatile instance_;
  static std::auto_ptr<RMOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};


#endif /* _APPOPTIONS_H */
