// $Id$

#include "Event_Logging_Service.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Log/EventLogFactory_i.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "tao/IORTable/IORTable.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

Event_Logging_Service::Event_Logging_Service (void)
  : service_name_ ("EventLogFactory"),
    ior_file_name_ (0),
    pid_file_name_ (0),
    bind_to_naming_service_ (true),
    nthreads_ (0)
{
  // No-Op.
}

Event_Logging_Service::~Event_Logging_Service (void)
{
  // No-Op.
}

void
Event_Logging_Service::init_ORB (int& argc, ACE_TCHAR *argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  CORBA::Object_var poa_object =
    this->orb_->resolve_initial_references("RootPOA");

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();
}

int
Event_Logging_Service::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:o:p:t:x"));
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = ACE_TEXT_ALWAYS_CHAR(get_opt.opt_arg ());
          break;

        case 'o':
          this->ior_file_name_ = get_opt.opt_arg ();
          break;

        case 'p':
          this->pid_file_name_ = get_opt.opt_arg ();
          break;

        case 't':
          this->nthreads_ = ACE_OS::atoi (get_opt.opt_arg ());
          break;

        case 'x':
          this->bind_to_naming_service_ = false;
          break;

        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
                      "-n service_name "
                      "-o ior_file_name "
                      "-p pid_file_name "
                      "-t threads "
                      "-x [disable naming service bind] "
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  return 0;
}

int
Event_Logging_Service::init (int argc, ACE_TCHAR *argv[])
{
  // initialize the ORB.
  this->init_ORB (argc, argv);

  if (this->parse_args (argc, argv) == -1)
    return -1;

  // Activate the event log factory
  ACE_NEW_THROW_EX (this->event_log_factory_,
                    TAO_EventLogFactory_i (),
                    CORBA::NO_MEMORY ());

  DsEventLogAdmin::EventLogFactory_var obj =
    this->event_log_factory_->activate (this->orb_.in (),
                                        this->poa_.in ());
  ACE_ASSERT (!CORBA::is_nil (obj.in ()));

  CORBA::String_var ior =
    this->orb_->object_to_string (obj.in ());

  if (true)
    {
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable");

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in ());

      adapter->bind("EventLogService", ior.in ());
    }

  if (this->ior_file_name_ != 0)
    {
      FILE* iorf = ACE_OS::fopen (this->ior_file_name_, ACE_TEXT("w"));
      if (iorf == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             this->ior_file_name_),
                            -1);
        }
      ACE_OS::fprintf (iorf, "%s\n", ior.in ());
      ACE_OS::fclose (iorf);
    }

  if (this->pid_file_name_ != 0)
    {
      FILE* pidf = ACE_OS::fopen (this->pid_file_name_, ACE_TEXT("w"));
      if (pidf != 0)
        {
          ACE_OS::fprintf (pidf,
                           "%ld\n",
                           static_cast<long> (ACE_OS::getpid ()));
          ACE_OS::fclose (pidf);
        }
    }

  if (this->bind_to_naming_service_)
    {
      // Resolve the naming service.
      this->resolve_naming_service ();

      // Register the Event Log Factory.
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_.c_str ());

      this->naming_->rebind (name, obj.in ());
    }

  return 0;
}

void
Event_Logging_Service::resolve_naming_service (void)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

int
Event_Logging_Service::run (void)
{
  if (this->nthreads_ > 0)
    {
      if (this->activate ((THR_NEW_LWP | THR_JOINABLE), this->nthreads_) != 0)
        return -1;

      this->thr_mgr ()->wait ();
      return 0;
    }

  this->orb_->run ();

  return 0;
}

int
Event_Logging_Service::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }

  return 0;
}

void
Event_Logging_Service::shutdown (void)
{
  if (this->bind_to_naming_service_)
    {
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_.c_str ());

      this->naming_->unbind (name);
    }

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}
