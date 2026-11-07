// -*- C++ -*-

//=============================================================================
/**
 *  @file   Logger_i.h
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 *  @author and Matthew Braun <mjb2@cec.wustl.edu>
 */
//=============================================================================


#ifndef TAO_ORBSVCS_LOGGER_I_H
#define TAO_ORBSVCS_LOGGER_I_H

#include <string>

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
  Logger_i (Logger_i const&) = delete;
  Logger_i (Logger_i&&) = delete;
  Logger_i& operator= (Logger_i const&) = delete;
  Logger_i& operator= (Logger_i&&) = delete;

  /// constructor
  explicit Logger_i (char const *name);

  /// destructor
  ~Logger_i () override = default;

  /// Writes the <log_rec> to the standard output.
  void log (Logger::Log_Record const &log_rec) override;

  /// Writes the <log_rec> to the standard output with the given
  /// verbosity level
  void logv (Logger::Log_Record const &log_rec,
             Logger::Verbosity_Level verbosity) override;

  /// Writes the <log_rec> to the standard output.
  void log_twoway (Logger::Log_Record const &log_rec) override;

  /// Writes the <log_rec> to the standard output with the given
  /// verbosity level
  void logv_twoway (Logger::Log_Record const &log_rec,
                    Logger::Verbosity_Level verbosity) override;

  /// Sets the verbosity level. Valid values are {VERBOSE, VERBOSE_LITE
  ///  and SILENT}. Defaults to VERBOSE
  void verbosity (Logger::Verbosity_Level level) override;

private:
  /// Converts the IDL defined <Log_Priority> enum type to the
  /// <ACE_Log_Priority> enum type.
  static ACE_Log_Priority priority_conversion (Logger::Log_Priority priority);

  /**
   * Converts the IDL defined <Verbosity_Level> enum type to a u_long,
   * which is used by the <ACE_Log_Record> to distinguish the
   * level of verbosity.
   */
  static u_long verbosity_conversion (Logger::Verbosity_Level verbosity_level);

  /// Logger identification.
  std::string name_;

  /// Keeps track of what our current verbosity level is.  This can be
  /// reset by the client to a new value at any point.
  Logger::Verbosity_Level verbosity_level_ {Logger::VERBOSE};
};

/**
 * @class Logger_Factory_i
 *
 * @brief Create a <Logger> of type <name>.
 */
class Logger_Factory_i : public virtual POA_Logger_Factory
{
public:
  Logger_Factory_i (Logger_Factory_i const&) = delete;
  Logger_Factory_i (Logger_Factory_i&&) = delete;
  Logger_Factory_i& operator= (Logger_Factory_i const&) = delete;
  Logger_Factory_i& operator= (Logger_Factory_i&&) = delete;

  /// Constructor.
  Logger_Factory_i () = default;

  /// Destructor.
  ~Logger_Factory_i () override = default;

  /**
   * This function returns a logger with name <name>. If <name> is
   * unique, a new logger is created; else, a previously created
   * logger of name <name> is returned
   */
  Logger_ptr make_logger (char const *name) override;

private:
  /**
   * Calls to <make_logger> will create a new instance of <Logger> and
   * bind into the hash map manager if <name> is unique, else it will
   * return a previously bound entry.
   */
  ACE_Hash_Map_Manager<ACE_CString, Logger_i *, ACE_Null_Mutex> hash_map_;
};

#endif /* TAO_ORBSVCS_LOGGER_I_H */
