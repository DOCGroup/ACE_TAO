#include "orbsvcs/Log_Macros.h"
#include "Notify_Logging_Service.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"
#include "orbsvcs/Notify/Service.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

#include <memory>

int
Notify_Logging_Service::init_ORB (int & argc, ACE_TCHAR * argv [])
{
  this->orb_ = CORBA::ORB_init (argc, argv);

  this->notify_service_ = TAO_Notify_Service::load_default ();

  if (this->notify_service_ == nullptr)
    {
      ORBSVCS_DEBUG ((LM_DEBUG, "Notify Service not found! check conf. file\n"));
      return -1;
    }

  CORBA::Object_var const poa_object =
    this->orb_->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil (poa_object.in ()))
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"),
                      -1);

  this->poa_ =
    PortableServer::POA::_narrow (poa_object.in ());

  PortableServer::POAManager_var const poa_manager =
    this->poa_->the_POAManager ();

  poa_manager->activate ();

  return 0;
}

int
Notify_Logging_Service::parse_args (int argc, ACE_TCHAR * argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:o:p:t:x"));
  int opt {};

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          this->service_name_ = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
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
          ORBSVCS_DEBUG ((LM_DEBUG,
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
Notify_Logging_Service::init (int argc, ACE_TCHAR * argv[])
{
  // initialize the ORB.
  if (this->init_ORB (argc, argv) != 0)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  this->notify_service_->init_service (this->orb_.in ());

  // Activate the factory
  this->notify_factory_ =
    notify_service_->create (this->poa_.in ());

  TAO_NotifyLogFactory_i *notify_log_factory {};
  ACE_NEW_THROW_EX (notify_log_factory,
                      TAO_NotifyLogFactory_i (this->notify_factory_.in ()),
                      CORBA::NO_MEMORY ());
  PortableServer::ServantBase_var const notify_log_factory_owner (notify_log_factory);

  DsNotifyLogAdmin::NotifyLogFactory_var const obj =
    notify_log_factory->activate (this->orb_.in (),
                                  this->poa_.in ());

  CORBA::String_var const ior =
    this->orb_->object_to_string (obj.in ());

  CORBA::Object_var const table_object =
    this->orb_->resolve_initial_references ("IORTable");

  IORTable::Table_var const adapter =
    IORTable::Table::_narrow (table_object.in ());

  adapter->bind ("NotifyLogService", ior.in ());

  if (this->ior_file_name_)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> const iorf (
        ACE_OS::fopen (this->ior_file_name_->c_str (), ACE_TEXT ("w")),
        ACE_OS::fclose);
      if (!iorf)
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             this->ior_file_name_->c_str ()),
                            -1);
        }

      ACE_OS::fprintf (iorf.get (), "%s\n", ior.in ());
    }

  if (this->pid_file_name_)
    {
      std::unique_ptr<FILE, int (*) (FILE *)> const pidf (
        ACE_OS::fopen (this->pid_file_name_->c_str (), ACE_TEXT ("w")),
        ACE_OS::fclose);
      if (pidf)
        {
          ACE_OS::fprintf (pidf.get (),
                           "%ld\n",
                           static_cast<long> (ACE_OS::getpid ()));
        }
    }

  if (this->bind_to_naming_service_)
    {
      // Resolve the naming service.
      this->resolve_naming_service ();

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->service_name_.c_str ());

      this->naming_->rebind (name,
                             obj.in ());
    }

  return 0;
}

void
Notify_Logging_Service::resolve_naming_service ()
{
  CORBA::Object_var const naming_obj =
    this->orb_->resolve_initial_references ("NameService");

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    throw CORBA::UNKNOWN ();

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in ());
}

int
Notify_Logging_Service::run ()
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
Notify_Logging_Service::svc ()
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
Notify_Logging_Service::shutdown ()
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
