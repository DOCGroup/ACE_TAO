#include "TLS_Client.h"

ACE_RCSID (Basic,
           TLS_Client,
           "$Id$")

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
TLS_Client::init (int argc, char *argv [] ACE_ENV_ARG_DECL)
{
  init_ORB (argc, argv ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  resolve_naming_service (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  resolve_TLS_Basic_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TLS_Client::init_ORB (int argc,
                      char *argv []
                      ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TLS_Client::resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TLS_Client::resolve_TLS_Basic_factory (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (BASIC_TLS_LOG_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->basic_log_factory_ =
    DsLogAdmin::BasicLogFactory::_narrow (obj.in ()
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TLS_Client::run_tests (ACE_ENV_SINGLE_ARG_DECL)
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
                                      logid
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
  basic_log->write_records (any_seq ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_n_records...\n"));
#ifndef ACE_LACKS_LONGLONG_T
  CORBA::Long retval = basic_log->get_n_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
#else
  CORBA::Long retval = basic_log->get_n_records (ACE_ENV_SINGLE_ARG_PARAMETER).lo();
  ACE_CHECK;
#endif

  ACE_DEBUG ((LM_DEBUG, "Number of records in Log = %d \n", retval));

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_current_size...\n"));
#ifndef ACE_LACKS_LONGLONG_T
  retval = basic_log->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
#else
  retval = basic_log->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER).lo();
  ACE_CHECK;
#endif

  ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d \n", retval));

  ACE_DEBUG ((LM_DEBUG, "Querying the Log: %s\n", QUERY_1));
  DsLogAdmin::Iterator_var iter_out;
  DsLogAdmin::RecordList_var rec_list =
    basic_log->query (QUERY_LANG, QUERY_1, iter_out);

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

  ACE_DEBUG ((LM_DEBUG,
              "Deleting records... \n"));

  retval = basic_log->delete_records (QUERY_LANG, QUERY_2 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Calling BasicLog::get_n_records...\n"));
#ifndef ACE_LACKS_LONGLONG_T
  retval = basic_log->get_n_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
#else
  retval = basic_log->get_n_records (ACE_ENV_SINGLE_ARG_PARAMETER).lo();
  ACE_CHECK;
#endif

  ACE_DEBUG ((LM_DEBUG, "Number of records in Log after delete = %d \n",
              retval));

  ACE_DEBUG ((LM_DEBUG, "Geting the current_size again...\n"));
#ifndef ACE_LACKS_LONGLONG_T
  retval = basic_log->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
#else
  retval = basic_log->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER).lo();
  ACE_CHECK;
#endif

  ACE_DEBUG ((LM_DEBUG, "Size of data in Log = %d \n", retval));

  basic_log->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
