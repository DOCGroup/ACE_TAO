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
  // This function creates and returns a logger with the given name.
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

private:
  char *name_;
  // Logger identification.
};

#endif /* TAO_ORBSVCS_LOGGER_I_H */
