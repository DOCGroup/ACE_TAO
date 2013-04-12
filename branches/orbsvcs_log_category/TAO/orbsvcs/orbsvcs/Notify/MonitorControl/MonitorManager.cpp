// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/MonitorControl/MonitorManager.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMonitor_i.h"

#include "orbsvcs/Naming/Naming_Client.h"

#include "tao/ORB.h"
#include "tao/IORTable/IORTable.h"

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_MonitorManager::TAO_MonitorManager (void)
  : run_ (false)
  , initialized_ (false)
{
}

int
TAO_MonitorManager::init (int argc, ACE_TCHAR* argv[])
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->task_.mutex_, -1);

  this->task_.argv_.add (ACE_TEXT("fake_process_name"));

  ACE_Get_Opt opts (argc,
                    argv,
                    ACE_TEXT ("o:"),
                    0,
                    0,
                    ACE_Get_Opt::PERMUTE_ARGS,
                    1);

  static const ACE_TCHAR* orbarg = ACE_TEXT ("ORBArg");
  static const ACE_TCHAR* nonamesvc = ACE_TEXT ("NoNameSvc");
  opts.long_option (orbarg, ACE_Get_Opt::ARG_REQUIRED);
  opts.long_option (nonamesvc, ACE_Get_Opt::NO_ARG);

  int c;
  while ((c = opts ()) != -1)
    switch (c)
      {
        case 'o':
          this->task_.ior_output_ = opts.opt_arg ();
          if (TAO_debug_level > 7)
            {
              ORBSVCS_DEBUG((LM_INFO,
                ACE_TEXT("(%P|%t) TAO_MonitorManager: Setting IOR output file to: %s"),
                  this->task_.ior_output_.c_str ()));
            }
          break;
        case 0:
          if (ACE_OS::strcmp (opts.long_option (), orbarg) == 0)
            {
              ACE_TCHAR * orbArgs = opts.opt_arg ();
              if (TAO_debug_level > 7)
                {
                  ORBSVCS_DEBUG((LM_INFO,
                    ACE_TEXT("(%P|%t) TAO_MonitorManager: Setting Orb arguments to: %s"),
                      orbArgs));
                }
              this->task_.argv_.add (orbArgs);
            }
          else if (ACE_OS::strcmp (opts.long_option (), nonamesvc) == 0)
            {
              if (TAO_debug_level > 7)
                {
                  ORBSVCS_DEBUG((LM_INFO,
                    ACE_TEXT("(%P|%t) TAO_MonitorManager: Not using naming service")));
                }
              this->task_.use_name_svc_ = false;
            }
          break;
        case ':':
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) TAO_MonitorManager: %s requires an argument\n"),
                             opts.last_option ()),
                             -1);
      }

  // Force the ARGV_T to copy the elements added by the add() method
  this->task_.argv_.argv ();

  // Rember that Monitor has been configured
  this->initialized_ = true;
  return 0;
}

int
TAO_MonitorManager::fini (void)
{
  if (!CORBA::is_nil (this->task_.orb_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->task_.mutex_, -1);

      if (!CORBA::is_nil (this->task_.orb_.in ()))
        {
          this->task_.orb_->shutdown (true);
        }
    }

  this->task_.wait ();
  return 0;
}

int
TAO_MonitorManager::run (void)
{
  bool activate = false;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->task_.mutex_, -1);

    // Work around for bug 3108. Need to create MC ORB in main thread
    // so any libs are loaded in the parent thread.
    // Initialize the ORB
    int argc = task_.argv_.argc ();
    task_.orb_ = CORBA::ORB_init (argc,
                                  task_.argv_.argv (),
                                  task_.mc_orb_name_.c_str ());

    if (!this->run_ && this->initialized_)
      {
        this->run_ = true;
        activate = true;
      }
  }

  int status = 0;

  if (activate)
    {
      status = this->task_.activate ();

      if (status == 0)
        {
          // cj: Wait till the child thread has initialized completely
          // It still leaves a tiny race window open, but hopefully not much.
          // The race condition R1 is in ORBTask::svc().

          this->task_.startup_barrier_.wait ();
        }
    }

  return status;
}

int
TAO_MonitorManager::resume (void)
{
  return this->run ();
}

void
TAO_MonitorManager::shutdown (void)
{
  TAO_MonitorManager* monitor =
    ACE_Dynamic_Service<TAO_MonitorManager>::instance (
      TAO_NOTIFY_MONITOR_CONTROL_MANAGER);

  if (monitor != 0)
    {
      monitor->fini ();
    }
}

TAO_MonitorManager::ORBTask::ORBTask (void)
 : use_name_svc_ (true)
   , startup_barrier_ (2) // synch the parent with the single child thread
   , mc_orb_name_ (ACE_TEXT("TAO_MonitorAndControl"))
{
}

int
TAO_MonitorManager::ORBTask::svc (void)
{
  try
    {
      if (CORBA::is_nil (this->orb_.in ()))
        {
          ORBSVCS_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) TAO_MonitorManager: Unable to "
                             "initialize the ORB\n"),
                            1);
        }

      PortableServer::POA_var poa;

      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->mutex_, -1);

        CORBA::Object_var obj =
          this->orb_->resolve_initial_references ("RootPOA");

        poa = PortableServer::POA::_narrow (obj.in ());

        if (CORBA::is_nil (poa.in ()))
          {
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) TAO_MonitorManager: Unable to "
                               "resolve the RootPOA\n"),
                              1);
          }

        PortableServer::POAManager_var poa_manager = poa->the_POAManager ();
        poa_manager->activate ();

        // Activate the object
        NotificationServiceMonitor_i* servant;
        ACE_NEW_RETURN (servant,
                        NotificationServiceMonitor_i (this->orb_.in ()), 1);
        PortableServer::ServantBase_var owner_transfer(servant);
        PortableServer::ObjectId_var id = poa->activate_object (servant);

        // Register the object with the IORTable
        obj = poa->id_to_reference (id.in ());
        CosNotification::NotificationServiceMonitorControl_var monitor =
          CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());
        CORBA::String_var ior = this->orb_->object_to_string (monitor.in ());
        obj = this->orb_->resolve_initial_references ("IORTable");
        IORTable::Table_var iortable = IORTable::Table::_narrow (obj.in ());

        if (CORBA::is_nil (iortable.in ()))
          {
            ORBSVCS_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) TAO_MonitorManager: Unable to "
                               "resolve the IORTable\n"),
                              1);
          }

        iortable->bind(ACE_TEXT_ALWAYS_CHAR(mc_orb_name_.c_str()), ior.in ());

        if (this->use_name_svc_)
          {
            TAO_Naming_Client nc;
            nc.init (this->orb_.in ());
            CosNaming::Name name (1);
            name.length (1);
            name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(mc_orb_name_.c_str()));
            nc->rebind (name, monitor.in ());
          }

        if (this->ior_output_.length () > 0)
          {
            FILE* fp = ACE_OS::fopen (this->ior_output_.c_str (), "w");

            if (fp == 0)
              {
                ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("(%P|%t) TAO_MonitorManager: "
                                   ACE_TEXT ("Unable to write to %s\n")),
                                   this->ior_output_.c_str ()),
                                  1);
              }
            else
              {
                ACE_OS::fprintf (fp, "%s", ior.in ());
                ACE_OS::fclose (fp);
              }
          }
      }

      // R1: race condition (partially fixed):
      // TAO_MonitorManager::fini() is called directly after
      // TAO_MonitorManager::run(), the shutdown call on the ORB could
      // happen but the ORB::run() loop won't exit.
      startup_barrier_.wait ();
      this->orb_->run ();

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->mutex_, -1);

      // Destroy the POA and ORB
      if (!CORBA::is_nil (poa.in ()))
        {
          poa->destroy (true, true);
        }

      this->orb_->destroy ();

      // Set to nil to avoid double shutdown in TAO_MonitorManager::fini()
      this->orb_ = CORBA::ORB::_nil ();
    }
  catch (const CORBA::Exception& ex)
    {
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          try
            {
              this->orb_->shutdown ();
            }
          catch (...)
            {
            }

          this->orb_ = CORBA::ORB::_nil ();
        }

      ex._tao_print_exception ("Caught in "
                               "TAO_MonitorManager::ORBTask::svc");
    }
  catch (...)
    {
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          try
            {
              this->orb_->shutdown ();
            }
          catch (...)
            {
            }
          this->orb_ = CORBA::ORB::_nil ();
        }

      ORBSVCS_ERROR ((LM_ERROR,
                  "Unexpected exception type caught "
                  "in TAO_MonitorManager::ORBTask::svc"));
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

typedef TAO_MonitorManager TAO_MonitorAndControl;
ACE_STATIC_SVC_DEFINE (TAO_MonitorAndControl,
                       ACE_TEXT (TAO_NOTIFY_MONITOR_CONTROL_MANAGER),
                       ACE_Service_Type::SERVICE_OBJECT,
                       &ACE_SVC_NAME (TAO_MonitorAndControl),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify_MC, TAO_MonitorAndControl)

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */
