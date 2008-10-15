/**
 *  @file   RMOptions.h
 *
 *  @brief Declared the RMOptions class interface.
 *
 */

#ifndef _RMOPTIONS_H
#define _RMOPTIONS_H

#include <string>

#include "ace/Thread_Mutex.h"
#include "ace/Auto_Ptr.h"

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
  bool proactive (void) const;
  double hertz (void) const;

protected:

  /// Constructor is protected to ensure Singleton access.
  RMOptions (void);

  double hertz_;
  bool proactive_;
  ArgPair arg_pair_;

  /// Singleton instance.
  static RMOptions * volatile instance_;
  static ACE_Auto_Ptr<RMOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};


#endif /* _RMOPTIONS_H */
