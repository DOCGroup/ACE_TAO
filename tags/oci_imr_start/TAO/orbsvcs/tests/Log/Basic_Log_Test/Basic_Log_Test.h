/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Basic_Log_Test.h
 *
 *  $Id$
 *
 *  An test of using the Basic_Logging_Service
 *
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef TAO_BASIC_LOG_TEST_H
#define TAO_BASIC_LOG_TEST_H

#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

class BasicLog_Test
{
   // = TITLE
   //   Class to test the BasicLog interface.
   // = DESCRIPTION
   //  This class exercises various methods of the DsLogAdmin::BasicLog
   //  interface.
 public:


  enum
  { MAX_LOG_SIZE = 8192 };

  BasicLog_Test(void);
  // constructor

  ~BasicLog_Test(void);
  // destructor

  int init(int argc, char *argv[]);

  int
  test_CreateLog(CORBA::ULongLong maxSize = MAX_LOG_SIZE);
  // 1. Test simple create for the log used by the other test
  //

int display_records ();

int 
delete_records (CORBA::ULongLong numberOfRecords);

 int
 test_LogAction ();
  // Method to test the LogAction operation.
  // 1. Set the max_size of the log to a value, MAX.
  // 2. Set the log_full_action to DsLogAdmin::halt
  // 3. Write MAX+1 records to the Log.
  // 4. Catch exception DsLogAdmin::LogFull.  Get the number of records.This should be MAX, the MAX+1'th record
  //    should have been discarded
  // 5. Get availability status should be log_full
  // 6. Set the log_full_action to DsLogAdmin::wrap
  // 7. Write MAX records to the Log.  Previous records should be over written.

  int
  test_adminState();
  // 1. set administrative state to DsLogAdmin::locked
  // 2. get administrative state.Is it locked
  // 3. write a record to the log.
  // 4. Catch exception DsLogAdmin::LogLocked

  int test_logSize();
  // 1. get current log size
  // 2. set log to less than current size.  Exception DsLogAdmin::InvalidParam should be thrown.
  // 3. set log to greater than current.

  int test_logCompaction(CORBA::ULong record_life = 1);
  // 1. get number of records in log. Should have records from prior test.
  //	If not write some.
  // 2. get maximum record life.
  // 3. set maximum record life. Wait record_life seconds.
  // 4. get maximum record life.
  // 5. get number of records in log.
  // 6. set maximum record life to entry value.

  int test_retrieval(CORBA::ULong numberOfRecordsToWrite = 1000);
  // 1. get time.
  // 2. write the records
  // 3. retrieve the records forwards. Compare to records written.
  // 4. retrieve the records backwards. Compare to records writen.
  // 5. repeat 3 and 4 using iterator.

  int test_query(CORBA::ULong numberOfRecords  = 1000 );
  // 1. write the records with NVList.
  // 2. query the records and compare the records

  int test_log_destroy();
  // 1. destroy the log.
  // 2. attemp to write to log. Should throw exception (?).


  int test_week_mask();

  int test_capacity_alarm_threshold();

private:
  // not implemented

  BasicLog_Test(const BasicLog_Test&);
  BasicLog_Test& operator=(const BasicLog_Test&);

  int
    init_factory (ACE_ENV_SINGLE_ARG_DECL);

  int
    parse_args();
  // command line argument parser

  int write_records(CORBA::ULongLong numberOfRecordsToWrite ACE_ENV_ARG_DECL_NOT_USED);
  // writes enough records to fill the log. Default write enough to fill log. Hopefully, not infinite.

  void
    resolve_basic_factory (ACE_ENV_SINGLE_ARG_DECL);
  // resolve log factory

  void
    resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // resolve nameing service

  void
    destroy_log();
  // destroys the is log is it exists

  int argc_;
  char **argv_;
  // command line

  char* logServiceIor_;

  DsLogAdmin::BasicLog_var basicLog_;
  // the log used for all of the test

  DsLogAdmin::BasicLogFactory_var factory_;
  // pointer to logging factory

  CORBA::ORB_var orb_;
  // pointer to the ORB

  CosNaming::NamingContext_var naming_context_;
  // naming context

  static const char* basic_log_factory_name_;

  static const char* naming_sevice_name_;
  // strings for log factory and naming service

};

#endif  /* TAO_BASIC_LOG_TEST_H */
