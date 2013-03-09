// $Id$

#include "TLS_Client.h"
#include "ace/Log_Msg.h"

#define NAMING_SERVICE_NAME "NameService"
#define BASIC_TLS_LOG_FACTORY_NAME "BasicLogFactory"
#define LOG_EVENT_COUNT 9
#define QUERY_1 "id < 5"
#define QUERY_2 "id >= 0"
#define QUERY_LANG "TCL"



TLS_Client::TLS_Client (void)
{
  // No-Op.
}

TLS_Client::~TLS_Client ()
{
  // No-Op.
}

void
TLS_Client::init (int argc, ACE_TCHAR *argv [])
{
  init_ORB (argc, argv);
  resolve_naming_service ();
  resolve_TLS_Basic_factory ();
}

void
TLS_Client::init_ORB (int argc,
                      ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);
}

void
TLS_Client::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
TLS_Client::resolve_TLS_Basic_factory (void)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (BASIC_TLS_LOG_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->basic_log_factory_ =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ());
}

void
TLS_Client::run_tests (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->basic_log_factory_.in ()));

  // create a log..

  ACE_DEBUG ((LM_DEBUG,
              "\nCalling BasicLogFactory::create...\n"));

  DsLogAdmin::LogFullActionType logfullaction = DsLogAdmin::halt;
  CORBA::ULongLong max_size = 0; // 0 means "infinite"

  DsLogAdmin::LogId logid = 0;

  DsLogAdmin::BasicLog_var basic_log =
    this->basic_log_factory_->create (logfullaction,
                                      max_size,
                                      logid);

  ACE_DEBUG ((LM_DEBUG,
              "Create returned logid = %d\n",logid));

  // Create some fake log events.
  DsLogAdmin::Anys any_seq (LOG_EVENT_COUNT);
  any_seq.length (LOG_EVENT_COUNT);

  for (int i = 0; i < LOG_EVENT_COUNT; i++)
    {
      any_seq [i] <<= "Foo";
    }

  ACE_DEBUG ((LM_DEBUG,
              "Writing %d records...\n", LOG_EVENT_COUNT));
  basic_log->write_records (any_seq);

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_n_records...\n"));
  CORBA::ULongLong retval = basic_log->get_n_records ();

  ACE_DEBUG ((LM_DEBUG, "Number of records in Log = %d\n", retval));

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_current_size...\n"));
  retval = basic_log->get_current_size ();

  ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

  ACE_DEBUG ((LM_DEBUG, "Querying the Log: %s\n", QUERY_1));
  DsLogAdmin::Iterator_var iter_out;
  DsLogAdmin::RecordList_var rec_list =
    basic_log->query (QUERY_LANG, QUERY_1, iter_out);

  CORBA::ULong j = 0;
  for (; j < rec_list->length();++j)  //dhanvey added info
  ACE_DEBUG ((LM_DEBUG,
              "id = %Q, time= %Q\n",
              rec_list[j].id, rec_list[j].time));

  ACE_DEBUG ((LM_DEBUG,
              "Deleting records...\n"));

  retval = basic_log->delete_records (QUERY_LANG, QUERY_2);

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_n_records...\n"));
  retval = basic_log->get_n_records ();

  ACE_DEBUG ((LM_DEBUG, "Number of records in Log after delete = %d\n",
              retval));

  ACE_DEBUG ((LM_DEBUG, "Geting the current_size again...\n"));
  retval = basic_log->get_current_size ();

  ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

  basic_log->destroy ();
}
