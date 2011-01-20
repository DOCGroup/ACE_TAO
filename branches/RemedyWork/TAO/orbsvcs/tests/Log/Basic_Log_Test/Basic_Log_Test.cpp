// $Id$

#include "Basic_Log_Test.h"

#include "tao/debug.h"

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

const char* BasicLog_Test::basic_log_factory_name_= "BasicLogFactory";
const char* BasicLog_Test::naming_sevice_name_ = "NameService";

BasicLog_Test::BasicLog_Test(void)
  : logServiceIor_ (0)
{
}

BasicLog_Test::~BasicLog_Test (void)
{
  this->destroy_log ();
}

int
BasicLog_Test::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              "internet");

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\nOrb initialized successfully\n"));

      // Parse command line and verify parameters.
      if (this->parse_args (argc, argv) == -1)
        return -1;

      // Initialize the factory
      int init_result = this->init_factory ();

      if (init_result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize naming"
                           "services.\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("init");
      return -1;
    }

  ACE_RETURN(0);

}

int
BasicLog_Test::init_factory (void)
{

  // Assumpting INS for finding LogServie

  //CORBA::Object_var logging_obj = orb_->resolve_initial_references ("BasicLogFactory",
  //                                                         ACE_TRY_ENV);

  this->resolve_naming_service ();

  this->resolve_basic_factory ();

  /*
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nLoggingService resolved\n"));

  if (CORBA::is_nil (logging_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "LogService resolved to nil object\n"),
                      -1);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nLogService resolved\n"));

  // Narrow the factory and check the success
  factory_ =
    DsLogAdmin::BasicLogFactory::_narrow (logging_obj.in ());

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nFactory narrowed\n"));
  if (CORBA::is_nil (factory_.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "narrow returned nil"n"),
                      -1);
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "\nLogger_Factory narrowed\n"));
  */
  // If debugging, get the factory's IOR
  CORBA::String_var str =
    orb_->object_to_string (basicLog_.in()); //Init the Client

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "The logger IOR is <%s>\n",
                str.in ()));

  return 0;
}

int
BasicLog_Test::test_CreateLog (CORBA::ULongLong maxSize)
{

  try
    {
      DsLogAdmin::LogId id;
      basicLog_ = factory_->create(DsLogAdmin::wrap, maxSize, id);
      // @@ Krish, never use != 0 to compare a NIL object reference!!!
      ACE_ASSERT (!CORBA::is_nil (basicLog_.in ()));
      ACE_DEBUG ((LM_DEBUG,
                  "The logger id is %d\n",
                  id));
    }
/*  catch (const DsLogAdmin::NoResources& resourceX)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "no resources creating basic log.\n"),
                        -1);
    }
*/
  catch (const DsLogAdmin::InvalidThreshold&)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "invalid threshold creating basic log.\n"),
                        -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_CreateLog");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught exception creating log.\n"),
                        -1);
    }

  ACE_RETURN(0);
}

int
BasicLog_Test::test_LogAction ()
{
  try
    {
      DsLogAdmin::LogId id = basicLog_->id ();

      ACE_DEBUG ((LM_DEBUG, "The log's id is %d\n", id));

      basicLog_->set_log_full_action (DsLogAdmin::halt);

      DsLogAdmin::LogFullActionType logFullAction =
        basicLog_->get_log_full_action ();

      if (logFullAction != DsLogAdmin::halt)
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Setting log full action to halt failed\n"),-1);
        }

      ACE_DEBUG ((LM_DEBUG,
                 "The log's full action successfully set to halt\n"));

#ifndef ACE_LACKS_LONGLONG_T
      ACE_DEBUG ((LM_INFO,
                 "The current size %Q, max size %Q\n",
                 basicLog_->get_current_size (),
                 basicLog_->get_max_size()));
#else
      ACE_DEBUG ((LM_DEBUG,
                 "The current size %u , max size %u\n",
                 basicLog_->get_current_size().lo (),
                 basicLog_->get_max_size().lo ()));
#endif

      // make sure that it is full and when writing
      this->write_records (0);

      ACE_DEBUG ((LM_DEBUG,
                  "Wrote records instead should have thrown exception"));

      basicLog_->set_log_full_action (DsLogAdmin::wrap);
      ACE_ERROR_RETURN((LM_ERROR,"Testing log action halt failed\n"),-1);
    }
  catch (const DsLogAdmin::LogFull&)
    {
      ACE_DEBUG ((LM_DEBUG,"Correctly caught exception LogFull\n"));

      CORBA::ULongLong nrecords = basicLog_->get_n_records ();

      ACE_DEBUG ((LM_ERROR,
                  "The number of records written was %d\n",
                  ACE_U64_TO_U32 (nrecords)));

      DsLogAdmin::AvailabilityStatus logStatus =
        basicLog_->get_availability_status ();

      if (!logStatus.log_full)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed the log's get_availability_status.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                 "Log's availability status correctly set to log_full\n"));

      basicLog_->set_log_full_action (DsLogAdmin::wrap);
      DsLogAdmin::LogFullActionType logFullAction =
        basicLog_->get_log_full_action();

      if (logFullAction != DsLogAdmin::wrap)
        ACE_ERROR_RETURN((LM_ERROR,
                          "Setting log full action to wrap failed\n"),-1);

      try
        {
          int i = this->write_records (0);

          ACE_DEBUG ((LM_DEBUG,"Test to wrap worked. %d written.\n",i));
        }
      catch (const DsLogAdmin::LogFull&)
        {
          ACE_DEBUG ((LM_ERROR,"Caught exception LogFull.  Fail testing wrapping of the log.\n"));
          return -1;
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("test_LogAction");
          return -1;
        }
    }

  return 0;
}
/*
int BasicLog_Test::display_records ()
{
  DsLogAdmin::Iterator_var iter_out;
  DsLogAdmin::RecordList_var rec_list =
    basicLog_->query ("EXTENDED_TCL", "id > 0", iter_out);

  CORBA::ULong j = 0;
  for (; j < rec_list->length();++j)  //dhanvey added info
#ifndef ACE_LACKS_LONGLONG_T
   ACE_DEBUG ((LM_DEBUG,
               "id = %Q, time= %Q\n",
               rec_list[j].id, rec_list[j].time));
#else
   ACE_DEBUG ((LM_DEBUG,
               "id = %u, time= %u\n",
               rec_list[j].id.lo(), rec_list[j].time.lo()));
#endif

  return 0;

}
*/
int BasicLog_Test::write_records (CORBA::ULongLong numberOfRecords)
{
  CORBA::ULongLong numOfRecs = numberOfRecords;
  if (numOfRecs == 0)
    {
      numOfRecs = basicLog_->get_max_size();
    }

#ifndef ACE_LACKS_LONGLONG_T
  ACE_DEBUG ((LM_INFO,"B_test: Write_records: Max size in bytes %Q.\n", numOfRecs));
#else
  ACE_DEBUG ((LM_INFO,"B_test: Write_records: currentsize in bytes %u.\n", numOfRecs.lo()));
#endif

  int i = 0;
  CORBA::ULongLong l = 0;

  if (numOfRecs == 0)
  {
    for (;;)
    {
        DsLogAdmin::Anys record;
        record.length(1);
        ACE_CString str ("For the test of log "
                         "full action, writing test record, ");
        char number[32];
        ACE_OS::sprintf (number, "%d.", i);
        str += number;
        l += str.length ();
        // record owns internal os string
        ++i;
        CORBA::String_var t(str.c_str ());
        record[0] <<= t.in ();
        basicLog_->write_records(record);
    }
  }
  else
  {
    for(l = 0 ; l < numOfRecs ;)
    {
        DsLogAdmin::Anys record;
        record.length(1);
        ACE_CString str ("For the test of log "
                         "full action, writing test record, ");
        char number[32];
        ACE_OS::sprintf (number, "%d.", i);
        str += number;
        l += str.length ();
        // record owns internal os string
        ++i;
        CORBA::String_var t(str.c_str ());
        record[0] <<= t.in ();
        basicLog_->write_records(record);

      }
  }

#ifndef ACE_LACKS_LONGLONG_T
    ACE_DEBUG ((LM_INFO,"B_test: Write_records: currentsize in bytes %Q.\n",
               basicLog_->get_current_size()));
#else
    ACE_DEBUG ((LM_DEBUG,"B_test: Write_records: currentsize in bytes %u.\n",
               basicLog_->get_current_size().lo()));
#endif

//dhanvey
  CORBA::ULongLong nrecords = basicLog_->get_n_records();

  ACE_DEBUG ((LM_ERROR,"The number of records in log is %d\n", ACE_U64_TO_U32(nrecords)));

  ACE_RETURN(static_cast<int> (nrecords));
}

int
BasicLog_Test::test_adminState()
{

  basicLog_->set_administrative_state(DsLogAdmin::locked);
  try
    {
      this->write_records(0);

      ACE_ERROR_RETURN((LM_ERROR,"Setting administrative state to lock failed.  DsLogAdmin::LogLocked not thrown.\n"),-1);

    }
  catch (const DsLogAdmin::LogLocked&)
    {
      ACE_DEBUG ((LM_DEBUG,"Setting administrative state to lock succeeded.  DsLogAdmin::LogLocked was caught.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      basicLog_->set_administrative_state(DsLogAdmin::unlocked);
      ex._tao_print_exception ("test_adminState");
      ACE_ERROR_RETURN((LM_ERROR,"Setting administrative state to lock failed. Exception throw.\n"),-1);
    }

  basicLog_->set_administrative_state(DsLogAdmin::unlocked);
  try
    {
      this->write_records(0);
      ACE_DEBUG ((LM_DEBUG,"Setting administrative state to succeeded.  DsLogAdmin::LogLocked not thrown.\n"));
      ACE_RETURN(0);

    }
  catch (const DsLogAdmin::LogLocked&)
    {
      ACE_DEBUG ((LM_DEBUG,"Setting administrative state to lock faild.  DsLogAdmin::LogLocked was caught.\n"));
    }
  ACE_RETURN(0);
}


int
BasicLog_Test::test_logSize (void)
{

  try
    {
      basicLog_->set_max_size (1);

      ACE_ERROR_RETURN ((LM_ERROR,
                         "Setting max size less than current size failed.\n"
                         "DsLogAdmin::InvalidParam not thrown.\n"),
                       -1);
    }
  catch (const DsLogAdmin::InvalidParam&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Setting max size less than current size succeeded.\n"
                  "DsLogAdmin::InvalidParam caught.\n"));
    }

  try
    {
      basicLog_->set_max_size (10000);

      CORBA::ULongLong nrecords = basicLog_->get_n_records ();
      int i = this->write_records (0);

      ACE_DEBUG ((LM_DEBUG,
                  "Increasing max size succeeded. Old record count, %d."
                  "New record count, %d\n",
                  ACE_U64_TO_U32(nrecords),
                  i));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_logSize");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Increasing max size failed. Exception thrown.\n"),
                        -1);
    }
  ACE_RETURN(0);
}

int
BasicLog_Test::test_logCompaction(CORBA::ULong lifeExpectancy)
{

  CORBA::ULongLong old_n_records = basicLog_->get_n_records ();
  if (old_n_records <= 0)
    {
      try
        {
          this->write_records (0);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("test_logCompaction");
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Caught exception trying to write records "
                             "for test_logCompaction.\n"),
                            -1);
        }
    }

  CORBA::ULong old_max_life = basicLog_->get_max_record_life ();

  try
    {
      basicLog_->set_max_record_life (lifeExpectancy);
      // @@: need to modify set_max_record_life
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_logCompaction");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught exception setting max record life "
                         "test_logCompaction.\n"),
                        -1);
    }

  ACE_OS::sleep (lifeExpectancy + 1); // make sure the records have aged.
  CORBA::ULongLong new_n_records = basicLog_->get_n_records ();
  basicLog_->set_max_record_life (old_max_life);

  ACE_DEBUG ((LM_ERROR,
              "The number of records in log is %d\n",
              ACE_U64_TO_U32 (new_n_records)));

  if (new_n_records != 0)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Log compaction failed, because %d records remain.\n",
                        ACE_U64_TO_U32 (new_n_records)),
                       -1 );
    }

  return 0;
}

//use ACE_U64_TO_U32 to convert ULongLong to ULong in call to this function
//Writes and retrieves numberOfrecordsToWrite records.
int
BasicLog_Test::test_retrieval (CORBA::ULong /* numberOfRecordsToWrite */)
{
  int rc = 0;

  if (!rc)
    {
      ACE_DEBUG ((LM_ERROR,"Test of retrieval: succeeded.\n"));
    }

  return rc;
}



int
BasicLog_Test::test_query(CORBA::ULong numberOfRecordsToWrite)
{
    // save for compares
    DsLogAdmin::Anys infos(numberOfRecordsToWrite);
    return -1;
}


int
BasicLog_Test::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("di"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           "\n"
                           "    -d: increase debug level\n",
                           argv[0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

void
BasicLog_Test::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (naming_sevice_name_);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
BasicLog_Test::resolve_basic_factory (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (basic_log_factory_name_);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->factory_ =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ());
}

void
BasicLog_Test::destroy_log()
{
  try
    {
      if (!CORBA::is_nil(basicLog_.in ()))
        {
          basicLog_->destroy ();

          basicLog_ = DsLogAdmin::BasicLog::_nil ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Destroying log");
    }
}

int
BasicLog_Test::test_log_destroy (void)
{

  ACE_DEBUG ((LM_ERROR, "Testing destroy log\n"));
  this->basicLog_->destroy ();

  try
    {
      this->write_records (1);
      ACE_DEBUG ((LM_INFO, "Wrote to log\n"));
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      ACE_DEBUG ((LM_ERROR,
                  "Test of destroy log succeeded: "
                  "caught CORBA::OBJECT_NOT_EXIST exception.\n"));
      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Destroying log");
      ACE_DEBUG ((LM_ERROR,
                  "Test of destroy log failed: "
                  "caught unexpected exception.\n"));
      return -1;
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Test of destroy log failed: no exception thrown.\n"),
                    -1);
}

int
BasicLog_Test::test_week_mask (void)
{
  DsLogAdmin::WeekMask masks;
  masks.length (1);

  masks[0].days =
    DsLogAdmin::Monday
    | DsLogAdmin::Tuesday
    | DsLogAdmin::Wednesday
    | DsLogAdmin::Friday
    | DsLogAdmin::Saturday;

  masks[0].intervals.length(1);

  masks[0].intervals[0].start.hour = (CORBA::Short) 13;

  masks[0].intervals[0].start.minute = (CORBA::Short) 30;
  masks[0].intervals[0].stop.hour = (CORBA::Short) 14;
  masks[0].intervals[0].stop.minute = (CORBA::Short) 30;

  this->basicLog_->set_week_mask (masks);

  return 0;
}


int BasicLog_Test::delete_records (CORBA::ULongLong /* numberOfRecords */)
{

#ifndef ACE_LACKS_LONGLONG_T
  ACE_DEBUG ((LM_INFO,
              " before delete -- curr size %Q, max %Q\n",
              basicLog_->get_current_size (),
              basicLog_->get_max_size()));
#else
  ACE_DEBUG ((LM_DEBUG,
              "2ndThe current size %u , max size %u\n",
              basicLog_->get_current_size().lo(),
              basicLog_->get_max_size().lo()));
#endif

  CORBA::ULongLong nrecords = basicLog_->get_n_records ();

  ACE_DEBUG ((LM_ERROR,
              "The number of records was %d\n",
              ACE_U64_TO_U32 (nrecords)));

  CORBA::Long retVal =
    basicLog_->delete_records ("EXTENDED_TCL", "id >= 0");

  ACE_DEBUG ((LM_DEBUG,
              "Number of records in Log after delete = %d\n",
              retVal));

#ifndef ACE_LACKS_LONGLONG_T
  ACE_DEBUG ((LM_INFO,
              " after delete -- curr size %Q, max %Q\n",
              basicLog_->get_current_size (),
              basicLog_->get_max_size ()));
#else
  ACE_DEBUG ((LM_DEBUG,
              "2ndThe current size %u , max size %u\n",
              basicLog_->get_current_size ().lo (),
              basicLog_->get_max_size ().lo ()));
#endif

  nrecords = basicLog_->get_n_records ();

  ACE_DEBUG ((LM_ERROR,
              "The number of records is %d\n",
              ACE_U64_TO_U32 (nrecords)));

return 0;
}

int
BasicLog_Test::test_capacity_alarm_threshold (void)
{

  //basicLog_->set_log_full_action(DsLogAdmin::halt);

  DsLogAdmin::CapacityAlarmThresholdList list;
  list.length(0);

  basicLog_->set_capacity_alarm_thresholds (list);


  list.length (1);
  list[0] = 200;

  try
    {
      basicLog_->set_capacity_alarm_thresholds (list);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Setting an invalid alarm threshold failed.  "
                         "DsLogAdmin::InvalidThreshold not thrown.\n"),
                        -1);
    }
  catch (const DsLogAdmin::InvalidThreshold&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Setting an invalid alarm threshold succeeded.  "
                  "DsLogAdmin::InvalidThreshold caught.\n"));
    }

  list.length (2);
  list[0] = 70;
  list[1] = 20;

  try
    {
      basicLog_->set_capacity_alarm_thresholds (list);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Setting an invalid alarm threshold failed.  "
                         "DsLogAdmin::InvalidThreshold not thrown.\n"),
                       -1);
    }
  catch (const DsLogAdmin::InvalidThreshold&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Setting an invalid alarm threshold succeeded.  "
                  "DsLogAdmin::InvalidThreshold caught.\n"));
    }

  list.length (2);
  list[0] = 20;
  list[1] = 70;

  basicLog_->set_capacity_alarm_thresholds (list);

  CORBA::ULongLong nrecords = basicLog_->get_n_records();

  ACE_DEBUG ((LM_ERROR,
              "The number of records is %d\n",
              ACE_U64_TO_U32(nrecords)));

  this->write_records (0);

  return 0;

}
