// $Id$
#include "Crash_Injector.h"
#include "Crash_Injector_Initializer.h"
#include "ace/Get_Opt.h"

#ifndef WIN32
#include <sys/time.h>
#endif

extern "C" void crash_handler(int)
{
    ACE_DEBUG((LM_DEBUG, "FTRT_Event_Service crashed\n"));
    exit(1);
}


namespace {
  Crash_Injector* injector;

  const char* types[] = {
    "immediate",
      "after",
      "timeout:"
  };
}

Crash_Injector::Crash_Injector()
: crash_type_(CRASH_IMMEDIATELY)
, num_of_invocation_(0)
, done_(false)
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_Crash_Injector);

  PortableInterceptor::ORBInitializer_var orb_initializer = new Crash_Injector_Initializer;

  PortableInterceptor::register_orb_initializer (orb_initializer.in ());
  injector = this;
}

Crash_Injector::~Crash_Injector()
{
}

Crash_Injector* Crash_Injector::instance()
{
  return injector;
}


int Crash_Injector::init (int argc, ACE_TCHAR* argv[] )
{
  ACE_Get_Opt get_opt(argc, argv, ACE_LIB_TEXT("n::o::t::"), 0, 0, ACE_Get_Opt::PERMUTE_ARGS, 1);
  get_opt.long_option(ACE_LIB_TEXT("num_invocation"), 'n', ACE_Get_Opt::ARG_OPTIONAL);
  get_opt.long_option(ACE_LIB_TEXT("operation"), 'o', ACE_Get_Opt::ARG_OPTIONAL);
  get_opt.long_option(ACE_LIB_TEXT("type"), 't', ACE_Get_Opt::ARG_OPTIONAL);

  int opt;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt) {
      case 'n':
        if (get_opt.opt_arg ())
          crash_after_number_of_invocation_ = ACE_OS::atoi(get_opt.opt_arg ());
        break;
      case 'o':
        if (get_opt.opt_arg ())

          operation_ = get_opt.opt_arg ();
        break;
      case 't':
        if (get_opt.opt_arg ()) {
          if (ACE_OS::strcasecmp(get_opt.opt_arg (), "immediate") == 0)
            crash_type_ = CRASH_IMMEDIATELY;
          else if (ACE_OS::strcasecmp(get_opt.opt_arg (), "after_reply") == 0)
            crash_type_ = CRASH_AFTER_REPLY;
          else if (ACE_OS::strncasecmp("timeout:", get_opt.opt_arg (), 8)==0)

          {
            crash_type_ = CRASH_TIMEOUT;
            this->time_to_crash_in_ms_  = ACE_OS::atoi(get_opt.opt_arg ()+8);
            if (this->time_to_crash_in_ms_ ==0)
              ACE_ERROR_RETURN((LM_ERROR, "Crash_Injector : Invalid timeout value\n"), -1);
          }
        }
        break;
      default:
        return -1;
    }
  }
  return 0;
}

void Crash_Injector::check_on_receive_request(const char* operation)
{
  if (crash_type_ < CRASH_AFTER_REPLY && ACE_OS::strcmp(operation_.c_str() ,operation) == 0 )
    if (crash_after_number_of_invocation_ == ++num_of_invocation_)
    {
      switch (crash_type_) {
        case CRASH_IMMEDIATELY:
          exit(1);
        case CRASH_TIMEOUT:
          this->crash_timeout();
      }
    }
}

void Crash_Injector::check_on_sending_reply(const char* operation)
{
  if (crash_type_ == CRASH_AFTER_REPLY && ACE_OS::strcmp(operation_.c_str() ,operation) == 0 )
  {
    if (crash_after_number_of_invocation_ == ++num_of_invocation_)
      done_ = true;
  }
}



void Crash_Injector::crash_timeout()
{
#ifndef WIN32
    if (time_to_crash > 0) {
      signal(SIGALRM, &crash_handler);
      struct itimerval in, out;
      in.it_value.tv_sec = time_to_crash/1000;
      in.it_value.tv_usec = (time_to_crash%1000)*1000;
      setitimer(ITIMER_REAL, &in, &out);
    }
    else
      exit(1);
#endif

}


ACE_FACTORY_DEFINE (CRASH, Crash_Injector)


ACE_STATIC_SVC_DEFINE (Crash_Injector,
    ACE_TEXT("Crash_Injector"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (Crash_Injector),
    ACE_Service_Type::DELETE_THIS
    | ACE_Service_Type::DELETE_OBJ,
    0)
