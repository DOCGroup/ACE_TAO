// -*- C++ -*-

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

#ifndef TAO_ORBSVCS_LOGGER_I_H
#define TAO_ORBSVCS_LOGGER_I_H

#include "LoggerS.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Null_Mutex.h"

class Logger_i : public virtual POA_Logger
{
  // = TITLE
  //   Used to log messages to a logging server.
public:
  Logger_i (const char* name);
  // constructor

  virtual ~Logger_i (void);
  // destructor

  virtual void log (const Logger::Log_Record &log_rec);
  // Writes the <log_rec> to the standard output.

  virtual void logv (const Logger::Log_Record &log_rec,
                     Logger::Verbosity_Level verbosity);
  // Writes the <log_rec> to the standard output with the given
  // verbosity level

  virtual void log_twoway (const Logger::Log_Record &log_rec);
  // Writes the <log_rec> to the standard output.

  virtual void logv_twoway (const Logger::Log_Record &log_rec,
                            Logger::Verbosity_Level verbosity);
  // Writes the <log_rec> to the standard output with the given
  // verbosity level

   void verbosity (Logger::Verbosity_Level level);
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

class Logger_Factory_i : public virtual POA_Logger_Factory
{
  // = TITLE
  //     Create a <Logger> of type <name>.
public:
  Logger_Factory_i (void);
  // Constructor.

  ~Logger_Factory_i (void);
  // Destructor.

  virtual Logger_ptr make_logger (const char *name);
  // This function returns a logger with name <name>. If <name> is
  // unique, a new logger is created; else, a previously created
  // logger of name <name> is returned

private:
  ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex> hash_map_;
  // Calls to <make_logger> will create a new instance of <Logger> and
  // bind into the hash map manager if <name> is unique, else it will
  // return a previously bound entry.
};

#endif /* TAO_ORBSVCS_LOGGER_I_H */
