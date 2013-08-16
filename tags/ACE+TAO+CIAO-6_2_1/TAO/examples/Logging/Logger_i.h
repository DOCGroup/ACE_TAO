// -*- C++ -*-

//=============================================================================
/**
 *  @file   Logger_i.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 *  @author and Matthew Braun <mjb2@cec.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ORBSVCS_LOGGER_I_H
#define TAO_ORBSVCS_LOGGER_I_H

#include "LoggerS.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/SString.h"
#include "ace/Null_Mutex.h"

/**
 * @class Logger_i
 *
 * @brief Used to log messages to a logging server.
 */
class Logger_i : public virtual POA_Logger
{
public:
  /// constructor
  Logger_i (const char* name);

  /// destructor
  virtual ~Logger_i (void);

  /// Writes the <log_rec> to the standard output.
  virtual void log (const Logger::Log_Record &log_rec);

  /// Writes the <log_rec> to the standard output with the given
  /// verbosity level
  virtual void logv (const Logger::Log_Record &log_rec,
                     Logger::Verbosity_Level verbosity);

  /// Writes the <log_rec> to the standard output.
  virtual void log_twoway (const Logger::Log_Record &log_rec);

  /// Writes the <log_rec> to the standard output with the given
  /// verbosity level
  virtual void logv_twoway (const Logger::Log_Record &log_rec,
                            Logger::Verbosity_Level verbosity);

  /// Sets the verbosity level. Valid values are {VERBOSE, VERBOSE_LITE
  ///  and SILENT}. Defaults to VERBOSE
   void verbosity (Logger::Verbosity_Level level);

private:
  /// Converts the IDL defined <Log_Priority> enum type to the
  /// <ACE_Log_Priority> enum type.
  ACE_Log_Priority priority_conversion (Logger::Log_Priority priority);

  /**
   * Converts the IDL defined <Verbosity_Level> enum type to a u_long,
   * which is used by the <ACE_Log_Record> to distinguish the
   * level of verbosity.
   */
  u_long verbosity_conversion (Logger::Verbosity_Level verbosity_level);

  /// Logger identification.
  char *name_;

  /// Keeps track of what our current verbosity level is.  This can be
  /// reset by the client to a new value at any point.
  Logger::Verbosity_Level verbosity_level_;
};

/**
 * @class Logger_Factory_i
 *
 * @brief Create a <Logger> of type <name>.
 */
class Logger_Factory_i : public virtual POA_Logger_Factory
{
public:
  /// Constructor.
  Logger_Factory_i (void);

  /// Destructor.
  ~Logger_Factory_i (void);

  /**
   * This function returns a logger with name <name>. If <name> is
   * unique, a new logger is created; else, a previously created
   * logger of name <name> is returned
   */
  virtual Logger_ptr make_logger (const char *name);

private:
  /**
   * Calls to <make_logger> will create a new instance of <Logger> and
   * bind into the hash map manager if <name> is unique, else it will
   * return a previously bound entry.
   */
  ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex> hash_map_;
};

#endif /* TAO_ORBSVCS_LOGGER_I_H */
