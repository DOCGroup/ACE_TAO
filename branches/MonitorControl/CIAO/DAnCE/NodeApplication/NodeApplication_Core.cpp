// $Id$

#include "DAnCE/Deployment/CIAO_NodeApplication_CallBackC.h"
#include "ace/Sched_Params.h"
#include "NodeApplication_Impl.h"
#include "NodeApplication_Core.h"
#include "ace/Null_Mutex.h"
#include "ciao/Server_init.h"
#include "ciao/CIAO_common.h"

#if !defined (__ACE_INLINE__)
# include "NodeApplication_Core.inl"
#endif /* __ACE_INLINE__ */

const char *
sched_policy_name (int sched_policy)
{
  const char *name = 0;

  switch (sched_policy)
    {
    case ACE_SCHED_OTHER:
      name = "SCHED_OTHER";
      break;
    case ACE_SCHED_RR:
      name = "SCHED_RR";
      break;
    case ACE_SCHED_FIFO:
      name = "SCHED_FIFO";
      break;
    }

  return name;
}


/// The following check is taken from $(TAO_ROOT)/tests/RTCORBA/
int
check_supported_priorities (CORBA::ORB_ptr orb)
{
  CIAO_TRACE ("NodeApplication_Core::check_supported_priorities");

  int const sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run,
  // i.e., more than 1 priority level.
  int const max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int const min_priority =
    ACE_Sched_Params::priority_min (sched_policy);

  if (max_priority == min_priority)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Not enough priority levels with the %s scheduling policy\n"
                  "on this platform to run, terminating program....\n"
                  "Check svc.conf options\n",
                  sched_policy_name (sched_policy)));

      return -1;
    }
  return 0;
}

int
CIAO::NodeApplication_Core::svc ()
{
  CIAO_TRACE ("CIAO::NodeApplication_Core::svc");

  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      // ...
      CIAO::NodeApplication_Impl *nodeapp_servant = 0;

      ACE_NEW_RETURN (nodeapp_servant,
                      CIAO::NodeApplication_Impl (this->orb_.in (),
                                                  root_poa.in (),
                                                  *this->configurator_.get ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (nodeapp_servant);

      // Configuring NodeApplication.
      PortableServer::ObjectId_var nodeapp_oid
        = root_poa->activate_object (nodeapp_servant);

      object = root_poa->id_to_reference (nodeapp_oid.in ());

      Deployment::NodeApplication_var nodeapp_obj =
        Deployment::NodeApplication::_narrow (object.in ());

      if (CORBA::is_nil (nodeapp_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to activate NodeApplication object\n"),
                          -1);

      /** @@ We need to call NodeApplication servant's init method.
       * But it's not sure to me where exactly we can get the
       * Properties needed by the init method at this moment. There are two
       * ways to set the property of the NodeApplication:
       * 1. call init remotely from NodeApplicationManager
       * 2. call init locally on the servant of NodeApplication.
       */
      bool retval = nodeapp_servant->init ();

      if (retval)
        {
          ACE_DEBUG ((LM_DEBUG, "NodeApplication Failed on creating and\
                               initializing the session container!"));
          return 1;
        }

      CORBA::String_var str = this->orb_->object_to_string (nodeapp_obj.in ());

      if (this->options_.write_ior_file ())
        CIAO::Utility::write_IOR (this->options_.ior_output_filename (),
                                  str.in ());

      // End Deployment part
      if (CIAO::debug_level () > 10)
        {
          ACE_DEBUG ((LM_INFO, "NodeApplication IOR: %s\n", str.in ()));

        }

      Deployment::NodeApplicationManager_var nodeapp_man;
      Deployment::Properties_var prop = new Deployment::Properties;

      if (this->options_.use_callback ())
        {
          object = this->orb_->string_to_object (this->options_.callback_ior ());

          CIAO::NodeApplication_Callback_var nam_callback
            = CIAO::NodeApplication_Callback::_narrow (object.in ());

          Deployment::Properties_out properties_out (prop.out ());

          nodeapp_man
            = nam_callback->register_node_application (nodeapp_obj.in (),
                                                       properties_out);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Running NodeApplication...\n"));

      this->orb_->run ();

      root_poa->destroy (1, 1);

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Exiting NodeApplication...\n"));
  return 0;
}

int
CIAO::NodeApplication_Core::startup (int argc, char *argv[])
{
  CIAO_TRACE ("CIAO::NodeApplication_Core::startup");

  // pre-init
  this->configurator_.reset (this->options_.create_nodeapp_configurator ());

  if (this->configurator_->create_config_managers ())
    return -1;

  if (this->configurator_->pre_orb_initialize ())
    return -1;

  // Initialize orb
  this->orb_ =
    CORBA::ORB_init (argc,
                     argv,
                     "");
  CIAO::Server_init (this->orb_.in ());

  return this->configurator_->post_orb_initialize (this->orb_.in ());
}

int
CIAO::NodeApplication_Core::run_orb ()
{
  CIAO_TRACE ("CIAO::NodeApplication_Core::run_orb");

  // check supported priority before running RT
  if (this->options_.rt_support () &&
      check_supported_priorities (this->orb_.in ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "ERROR: DISABLE RT SUPPORT\n"), -1);
      //      this->options_.rt_support_ = 0;
    }

  if (this->options_.rt_support ()) // RT support reuqested
    {


      // spawn a thread
      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int result =
        this->activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (this->orb_->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            return -1;
        }

      ACE_DEBUG ((LM_DEBUG, "*** Running NodeApplication in RT mode\n"));

      // Wait for task to exit.
      result =
        this->wait ();
      ACE_ASSERT (result != -1);

      ACE_DEBUG ((LM_INFO, "*** Ending  NodeApplication\n"));
      return 0;

    }
  else
    return this->svc ();
}

