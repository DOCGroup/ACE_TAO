/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Logging_Strategy.h
//
// = AUTHOR
//    Prashant Jain 
// 
// ============================================================================

#if !defined (ACE_LOGGING_STRATEGY_H)
#define ACE_LOGGING_STRATEGY_H

#include "ace/Service_Object.h"

class ACE_Logging_Strategy : public ACE_Service_Object
  // = TITLE
  //     This class provides the hooks to control the output produced
  //     by any of the network services. 
  // 
  // = DESCRIPTION
  //   	 Depending upon when this service is invoked and with what
  //     flags, the output of other network services can be
  //     controlled. The output can be streamed to stderr, to a file,
  //     to a logging daemon, or it can be set to be "silent".
{
public:
  virtual int init (int argc, char *argv[]);
  // Dynamic linking hook.

  int parse_args (int argc, char *argv[]);
  // Parse svc.conf arguments.
private:
  void tokenize (char *flag_string);
  // Tokenize to set all the flags
  u_long flags_;
  char *filename_;
};

ACE_SVC_FACTORY_DECLARE (ACE_Logging_Strategy)

#endif /* ACE_LOGGING_STRATEGY_H */
