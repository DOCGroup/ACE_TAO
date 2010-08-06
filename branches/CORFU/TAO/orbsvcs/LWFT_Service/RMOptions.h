// -*- C++ -*-
// $Id$

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

class RMOptions
/// TITLE
///   Singleton class for the program options.
{
public:
  /// Singleton access method.
  static RMOptions *instance (void);

  bool parse_args (int &argc, char **argv);
  
  /// Member accessors.
  bool proactive (void) const;
  double hertz (void) const;
  bool static_mode (void) const;
  bool use_naming_service (void) const;

protected:
  /// Constructor is protected to ensure Singleton access.
  RMOptions (void);

  double hertz_;
  bool proactive_;
  bool static_mode_;
  bool use_naming_service_;

  /// Singleton-related stuff.
  static RMOptions * volatile instance_;
  static ACE_Auto_Ptr<RMOptions> deleter_;
  static ACE_Thread_Mutex lock_;
};

#endif /* _RMOPTIONS_H */
