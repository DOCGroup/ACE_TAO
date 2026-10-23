#include "TLS_Client.h"
#include "ace/Log_Msg.h"

namespace
{
  constexpr char naming_service_name[] = "NameService";
  constexpr char basic_tls_log_factory_name[] = "BasicLogFactory";
  constexpr int log_event_count = 9;
  constexpr char query_1[] = "id < 5";
  constexpr char query_2[] = "id >= 0";
  constexpr char query_lang[] = "TCL";
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
TLS_Client::resolve_naming_service ()
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (naming_service_name);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

void
TLS_Client::resolve_TLS_Basic_factory ()
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (basic_tls_log_factory_name);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name);

  this->basic_log_factory_ =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ());
}

void
TLS_Client::run_tests ()
{
  ACE_ASSERT (!CORBA::is_nil (this->basic_log_factory_.in ()));

  // create a log..

  ACE_DEBUG ((LM_DEBUG,
              "\nCalling BasicLogFactory::create...\n"));

  DsLogAdmin::LogFullActionType logfullaction = DsLogAdmin::halt;
  CORBA::ULongLong max_size {}; // 0 means "infinite"

  DsLogAdmin::LogId logid {};

  DsLogAdmin::BasicLog_var basic_log =
    this->basic_log_factory_->create (logfullaction,
                                      max_size,
                                      logid);

  ACE_DEBUG ((LM_DEBUG,
              "Create returned logid = %d\n",logid));

  // Create some fake log events.
  DsLogAdmin::Anys any_seq (log_event_count);
  any_seq.length (log_event_count);

  for (int i = 0; i < log_event_count; i++)
    {
      any_seq [i] <<= "Foo";
    }

  ACE_DEBUG ((LM_DEBUG,
              "Writing %d records...\n", log_event_count));
  basic_log->write_records (any_seq);

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_n_records...\n"));
  CORBA::ULongLong retval = basic_log->get_n_records ();

  ACE_DEBUG ((LM_DEBUG, "Number of records in Log = %d\n", retval));

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_current_size...\n"));
  retval = basic_log->get_current_size ();

  ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d\n", retval));

  ACE_DEBUG ((LM_DEBUG, "Querying the Log: %s\n", query_1));
  DsLogAdmin::Iterator_var iter_out;
  DsLogAdmin::RecordList_var rec_list =
    basic_log->query (query_lang, query_1, iter_out);

  CORBA::ULong j {};
  for (; j < rec_list->length();++j)  //dhanvey added info
  ACE_DEBUG ((LM_DEBUG,
              "id = %Q, time= %Q\n",
              rec_list[j].id, rec_list[j].time));

  ACE_DEBUG ((LM_DEBUG,
              "Deleting records...\n"));

  retval = basic_log->delete_records (query_lang, query_2);

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
