/* -*- C++ -*- */

// ============================================================================
//
// $Id$
//
// = LIBRARY
//    TAO services
// 
// = FILENAME
//   Logger_i.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>,
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>, and
//    Matthew Braun <mjb2@cec.wustl.edu> 
//
// ============================================================================

#if !defined (TAO_ORBSVCS_LOGGER_I_H)
#define TAO_ORBSVCS_LOGGER_I_H

#include "orbsvcs/LoggerS.h"

class TAO_ORBSVCS_Export Logger_Factory_i : public virtual POA_Logger_Factory
{
  // = TITLE
  //     Create a <Logger> of type <name>.
public:
  Logger_Factory_i (void);
  // constructor.

  virtual Logger_ptr make_logger (const char *name,
                                  CORBA::Environment &_env);
  // This function creates and returns a logger with the given <name>.
  // Currently, <name> is unused.
};

class Logger_i : public virtual POA_Logger
{
  // = TITLE
  //   Used to log messages to a logging server.
public:
  Logger_i (const char* name);
  // constructor

  virtual ~Logger_i (void);
  // destructor

  virtual void log (const Logger::Log_Record &log_rec,
		    CORBA::Environment &_env);
  // Writes the <log_rec> to the standard output.

  // @@ Matt, please add the method for verbosity().

private:
  ACE_Log_Priority priority_conversion (Logger::Log_Priority priority);
  // Converts the IDL defined <Log_Priority> enum type to the
  // <ACE_Log_Priority> enum type.
  
  u_long verbosity_conversion (Logger::Log_Verbosity verbosity);
  // Converts the IDL defined <Log_Verbosity> enum type to a u_long
  // which is used by the <ACE_Log_Record> to distinguish the
  // verbosity level
  
  char *name_;
  // Logger identification.

  Logger::Log_Verbosity verbosity_level_;
  // Keeps track of what our current verbosity level is.  This can be
  // reset by the client to a new value at any point.
};

#endif /* TAO_ORBSVCS_LOGGER_I_H */
