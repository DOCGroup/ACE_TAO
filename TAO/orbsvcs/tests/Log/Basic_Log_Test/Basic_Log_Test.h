/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Basic_Log_Test.h
 *
 *  An test of using the Basic_Logging_Service
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef TAO_BASIC_LOG_TEST_H
#define TAO_BASIC_LOG_TEST_H

#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

/// Tests the BasicLog interface by exercising operations on
/// `DsLogAdmin::BasicLog`.
class BasicLog_Test
{
 public:
  static constexpr CORBA::ULongLong MAX_LOG_SIZE = 8192;

  BasicLog_Test () = default;

  ~BasicLog_Test();

  BasicLog_Test (BasicLog_Test const &) = delete;
  BasicLog_Test (BasicLog_Test &&) = delete;
  BasicLog_Test &operator= (BasicLog_Test const &) = delete;
  BasicLog_Test &operator= (BasicLog_Test &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);

  /// Creates the log used by the other tests.
  int test_CreateLog (CORBA::ULongLong maxSize = MAX_LOG_SIZE);

  int display_records ();

  int delete_records (CORBA::ULongLong numberOfRecords);

  /// Tests LogAction halt and wrap behavior, including overflow handling,
  /// record count, and availability status.
  int test_LogAction ();

  /// Verifies that a locked log rejects writes with LogLocked.
  int test_adminState ();

  /// Checks the current log size and verifies that setting it below the
  /// current size raises InvalidParam.
  int test_logSize();

  /// Checks the record count before and after compaction at the specified
  /// record lifetime, then restores the original lifetime.
  int test_logCompaction(CORBA::ULong record_life = 1);

  /// Writes records and retrieves them forwards and backwards, directly and
  /// through an iterator.
  int test_retrieval(CORBA::ULong numberOfRecordsToWrite = 1000);

  /// Writes records using an NVList and compares them with query results.
  int test_query(CORBA::ULong numberOfRecords  = 1000);

  /// Destroys the log and verifies that it can no longer be written to.
  int test_log_destroy();

  int test_week_mask();

  int test_capacity_alarm_threshold();

private:
  int init_factory ();

  /// Parses the command-line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Writes records until the log is full.
  int write_records(CORBA::ULongLong numberOfRecordsToWrite);

  /// Resolves the log factory.
  void resolve_basic_factory ();

  /// Resolves the naming service.
  void resolve_naming_service ();

  /// Destroys the log if it exists.
  void destroy_log ();

  /// The log used by the tests.
  DsLogAdmin::BasicLog_var basicLog_ {};

  /// The logging factory.
  DsLogAdmin::BasicLogFactory_var factory_ {};

  /// The ORB.
  CORBA::ORB_var orb_ {};

  /// The naming context.
  CosNaming::NamingContext_var naming_context_ {};

  /// Names for the log factory and naming service.
  static constexpr char basic_log_factory_name_[] = "BasicLogFactory";
  static constexpr char naming_service_name_[] = "NameService";
};

#endif  /* TAO_BASIC_LOG_TEST_H */
