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

  virtual void logv (const Logger::Log_Record &log_rec,
                     Logger::Verbosity_Level verbosity,
                     CORBA::Environment &_env);
  // Writes the <log_rec> to the standard output with the given
  // verbosity level

  Logger::Verbosity_Level verbosity (void) const;
  // Gets the verbosity level

  void verbosity (Logger::Verbosity_Level level, CORBA::Environment &env);
  // Sets the verbosity level. Valid values are {VERBOSE, VERBOSE_LITE
  //  and SILENT}. Defaults to VERBOSE

private:
  ACE_Log_Priority priority_conversion (Logger::Log_Priority priority);
  // Converts the IDL defined <Log_Priority> enum type to the
  // <ACE_Log_Priority> enum type.

  u_long verbosity_conversion (Logger::Verbosity_Level verbosity_level);
  // Converts the IDL defined <Verbosity_Level> enum type to a u_long,
  // which is used by the <ACE_Log_Record> to distinguish the
  // level of verbosity.

  char *name_;
  // Logger identification.

  Logger::Verbosity_Level verbosity_level_;
  // Keeps track of what our current verbosity level is.  This can be
  // reset by the client to a new value at any point.
};

class TAO_ORBSVCS_Export Logger_Factory_i : public virtual POA_Logger_Factory
{
  // = TITLE
  //     Create a <Logger> of type <name>.
public:
  Logger_Factory_i (void);
  // Constructor.

  ~Logger_Factory_i (void);
  // Destructor.

  virtual Logger_ptr make_logger (const char *name,
                                  CORBA::Environment &_env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // This function returns a logger with name <name>. If <name> is
  // unique, a new logger is created; else, a previously created
  // logger of name <name> is returned

private:
  ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex> hash_map_;
  // Calls to <make_logger> will create a new instance of <Logger> and
  // bind into the hash map manager if <name> is unique, else it will
  // return a previously bound entry.
};

// I'm going to define these here for testing with the intent of 
// migration into ace/OS.h
# if defined(ACE_NEW_THROWS_EXCEPTIONS)
#   define ACE_NEW_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
do { try { POINTER = new CONSTRUCTOR; } \
 catch (bad_alloc) { errno = ENOMEM; TAO_THROW (EXCEPTION); } \
} while (0)
  
#   define ACE_NEW_THROW_RETURN(POINTER,CONSTRUCTOR,EXCEPTION,RET_VAL) \
do { try { POINTER = new CONSTRUCTOR; } \
 catch (bad_alloc) { errno = ENOMEM; TAO_THROW_RETURN (EXCEPTION,RET_VAL); } \
} while (0)
# else
#   define ACE_NEW_THROW(POINTER,CONSTRUCTOR,EXCEPTION) \
do { POINTER = new CONSTRUCTOR; \
 if (POINTER == 0) { errno = ENOMEM; TAO_THROW (EXCEPTION); } \
} while (0)
#   define ACE_NEW_THROW_RETURN(POINTER,CONSTRUCTOR,EXCEPTION,RET_VAL) \
do { POINTER = new CONSTRUCTOR; \
 if (POINTER == 0)\
  { errno = ENOMEM; TAO_THROW_RETURN (EXCEPTION,RET_VAL); } \
} while (0)
# endif /* ACE_NEW_THROWS_EXCEPTIONS */

#endif /* TAO_ORBSVCS_LOGGER_I_H */
