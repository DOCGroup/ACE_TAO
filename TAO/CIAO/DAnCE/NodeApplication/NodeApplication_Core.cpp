// $Id$

#include "ciao/NodeApplication_CallBackC.h"
#include "ace/Sched_Params.h"
#include "NodeApplication_Impl.h"
#include "NodeApplication_Core.h"
#include "ace/Null_Mutex.h"
#include "ciao/CIAO_common.h"
#include "ace/Get_Opt.h"

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
  int sched_policy =
    orb->orb_core ()->orb_params ()->ace_sched_policy ();

  // Check that we have sufficient priority range to run,
  // i.e., more than 1 priority level.
  int max_priority =
    ACE_Sched_Params::priority_max (sched_policy);
  int min_priority =
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

CIAO::NoOp_Configurator::~NoOp_Configurator (void)
{
  // Not much to do.
}

int
CIAO::NoOp_Configurator::initialize (void)
{
  // Do nothing to initialize the NodeApplication process.
  return 0;
}

int
CIAO::NoOp_Configurator::init_resource_manager
(const ::Deployment::Properties & /*properties*/)
{
  // @@ Currently do thing.  We should go over the resource struct in
  // the future and throw exceptions if there are
  // un-recognizable/supported stuff in it.
  return 0;
}

CORBA::PolicyList *
CIAO::NoOp_Configurator::find_container_policies
(const ::Deployment::Properties & /*properties*/)
{
  // Not much to do.

  return 0;
}

int
CIAO::NodeApplication_Options::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "nrk:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        this->use_callback_ = 0;
        break;


      case 'o':  // get the file name to write to
        this->ior_output_filename_ = get_opts.opt_arg ();
        break;

      case 'k':  // get the activator callback IOR
        this->callback_ior_ = get_opts.opt_arg ();
        break;

      case 'r':                 // Enable RT support
        this->rt_support_ = 1;
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "-n Do not use Callback (for testing)\n"
                           "-o <ior_output_file>\n"
                           "-k <NodeApplicationManager_callback_ior>\n"
                           "-r Request RT support\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (this->use_callback_ && this->callback_ior_.length() == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Callback IOR to NodeApplicationManager is required.\n"),
                      -1);

  return 0;
}

int
CIAO::NodeApplication_Core::svc ()
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ...
      CIAO::NodeApplication_Impl *nodeapp_servant;

      ACE_NEW_RETURN (nodeapp_servant,
                      CIAO::NodeApplication_Impl (this->orb_.in (),
                                                  root_poa.in (),
                                                  *this->configurator_.get ()),
                      -1);

      PortableServer::ServantBase_var safe_servant (nodeapp_servant);

      // Configuring NodeApplication.
      PortableServer::ObjectId_var nodeapp_oid
        = root_poa->activate_object (nodeapp_servant
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object = root_poa->id_to_reference (nodeapp_oid.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Deployment::NodeApplication_var nodeapp_obj =
        Deployment::NodeApplication::_narrow (object.in ()
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
      bool retval = nodeapp_servant->init (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      if (retval)
      {
	ACE_DEBUG ((LM_DEBUG, "NodeApplication Failed on creating and\
                               initializing the session container!"));
	return 1;
      }
      
      CORBA::String_var str = this->orb_->object_to_string (nodeapp_obj.in ()
                                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
      if (this->options_.ior_output_filename_.length () != 0)
        CIAO::Utility::write_IOR (this->options_.ior_output_filename_.c_str (),
                                  str.in ());

      // End Deployment part
      if (CIAO::debug_level () > 10)
        {
          ACE_DEBUG ((LM_INFO, "NodeApplication IOR: %s\n", str.in ()));

        }

      Deployment::NodeApplicationManager_var nodeapp_man;
      Deployment::Properties_var prop = new Deployment::Properties;

      if (this->options_.use_callback_)
        {
          object = this->orb_->string_to_object (this->options_.callback_ior_.c_str ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CIAO::NodeApplication_Callback_var nam_callback
            = CIAO::NodeApplication_Callback::_narrow (object.in ()
						       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Deployment::Properties_out properties_out (prop.out ());

          nodeapp_man
            = nam_callback->register_node_application (nodeapp_obj.in (),
                                                       properties_out
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Running NodeApplication...\n"));

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "Exiting NodeApplication...\n"));
  return 0;
}

int
CIAO::NodeApplication_Core::run_orb ()
{
  // check supported priority before running RT
  if (this->options_.rt_support_ != 0 &&
      check_supported_priorities (this->orb_.in ()) != 0)
    {
      ACE_DEBUG ((LM_ERROR, "ERROR: DISABLE RT SUPPORT\n"));
      this->options_.rt_support_ = 0;
    }

  if (this->configurator_factory () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ERROR: Unable to acquire and initialize the NodeApplication configurator.\n"), -1);

  if (this->options_.rt_support_ != 0) // RT support reuqested
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

int
CIAO::NodeApplication_Core::configurator_factory (void)
{
  CIAO::NodeApp_Configurator* ptr = 0;

  if (this->options_.rt_support_)
    {
      // Manufacture a RTNA_configurator
    }
  else
    ACE_NEW_RETURN (ptr,
                    CIAO::NoOp_Configurator (this->orb_.in ()),
                    -1);

  this->configurator_.reset (ptr);

  return this->configurator_->initialize ();
}
