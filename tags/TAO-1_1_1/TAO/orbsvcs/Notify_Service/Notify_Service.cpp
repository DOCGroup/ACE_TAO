// $Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "Notify_Service.h"

Notify_Service::Notify_Service (void)
  : bootstrap_ (0),
    use_name_svc_ (1),
    ior_output_file_ (0),
    notify_factory_name_ (NOTIFY_KEY),
    notify_channel_name_ (NOTIFY_CHANNEL_NAME),
    register_event_channel_ (0)
{
  // No-Op.
}

Notify_Service::~Notify_Service (void)
{
  if (ior_output_file_)
    fclose(ior_output_file_);
}

int
Notify_Service::init_ORB  (int& argc, char *argv [],
                             CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var poa_obj  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (poa_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the RootPOA.\n"),
                      -1);

  this->poa_ =
    PortableServer::POA::_narrow (poa_obj.in (),
                                  ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  PortableServer::POAManager_var poa_manager =
    this->poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Notify_Service::startup (int argc, char *argv[],
                          CORBA::Environment &ACE_TRY_ENV)
{
  // initalize the ORB.
  if (this->init_ORB (argc, argv,
                      ACE_TRY_ENV) != 0)
  return -1;

  ACE_CHECK_RETURN (-1);

  if (this->parse_args(argc, argv) != 0)
    return -1;

  // Check first if the naming service
  if (this->use_name_svc_)
    {
      // Resolve the naming service.
      if (this->resolve_naming_service (ACE_TRY_ENV) != 0)
        return -1;

      ACE_CHECK_RETURN (-1);
  }

  ACE_DEBUG ((LM_DEBUG,
              "\nStarting up the Notification Service...\n"));

  // Activate the factory
  this->notify_factory_ =
    TAO_Notify_EventChannelFactory_i::create (this->poa_.in (),
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));

  // Write IOR to a file, if asked.
  CORBA::String_var str =
    this->orb_->object_to_string (this->notify_factory_.in (), ACE_TRY_ENV);

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
      this->ior_output_file_ = 0;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
                "The Notification Event Channel Factory IOR is <%s>\n",
                str.in ()));
  }

  // Make it bootstrappable, if asked.
  if (this->bootstrap_)
    {
    // add to Object key-IOR table
      if (this->orb_->_tao_add_to_IOR_table (this->notify_factory_name_.c_str(),
                                           this->notify_factory_.in ()) != 0)
      ACE_ERROR_RETURN((LM_ERROR,
                        "Unable to add IOR to table\n"), -1);
  }

  // Register with the Name service, if asked
  if (this->use_name_svc_)
  {
    // Register the Factory
    ACE_ASSERT(!CORBA::is_nil (this->naming_.in ()));

    CosNaming::Name name (1);
    name.length (1);
    name[0].id =
      CORBA::string_dup (this->notify_factory_name_.c_str ());

    this->naming_->rebind (name,
                           this->notify_factory_.in (),
                           ACE_TRY_ENV);
    ACE_CHECK_RETURN (-1);

    ACE_DEBUG ((LM_DEBUG,
                "Registered with the naming service as: %s\n",
                this->notify_factory_name_.c_str()));

    if (this->register_event_channel_ == 1)
      {
        // create an event channel
        CosNotifyChannelAdmin::ChannelID id;

        CosNotification::QoSProperties initial_qos;
        CosNotification::AdminProperties initial_admin;

        CosNotifyChannelAdmin::EventChannel_var ec =
          this->notify_factory_->create_channel (initial_qos,
                                                 initial_admin,
                                                 id,
                                                 ACE_TRY_ENV);
        name[0].id =
          CORBA::string_dup (this->notify_channel_name_.c_str ());

        this->naming_->rebind (name,
                               ec.in (),
                               ACE_TRY_ENV);
        ACE_CHECK_RETURN (-1);

        ACE_DEBUG ((LM_DEBUG,
                    "Registered an Event Channel with the naming service as: %s\n",
                    this->notify_channel_name_.c_str()));

      }
  }

  return 0;
}

int
Notify_Service::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references ("NameService",
                                            ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to resolve the Naming Service.\n"),
                      -1);

  this->naming_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Notify_Service::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "%s: Running the Notification Service\n",
              __FILE__));
  if (this->orb_->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  return 0;
}

void
Notify_Service::shutdown (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate.
  PortableServer::ObjectId_var oid =
    this->poa_->reference_to_id (this->notify_factory_.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in (),
                                 ACE_TRY_ENV);
  ACE_CHECK;

  if (this->use_name_svc_)
  {
    // Unbind from the naming service.
    CosNaming::Name name (1);
    name.length (1);
    name[0].id = CORBA::string_dup (this->notify_factory_name_.c_str ());

    this->naming_->unbind (name,
                           ACE_TRY_ENV);
  }

  // shutdown the ORB.
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}


int
Notify_Service::parse_args(int argc, char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc, argv);

    char *current_arg = 0;
    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-Factory")))
        {
          this->notify_factory_name_.set(current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Boot") == 0)
        {
          this->bootstrap_ = 1;
          // Make it bootstrappable

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-NameSvc") == 0)
        {
          this->use_name_svc_ = 1;
          // Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-NoNameSvc") == 0)
        {
          this->use_name_svc_ = 0;
          // Don't Register ref with the name service

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-IORoutput")))
        {
          this->ior_output_file_ = ACE_OS::fopen (current_arg, "w");
          if (this->ior_output_file_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s for writing: %p\n",
                               current_arg), -1);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ChannelName")))
        {
          this->notify_channel_name_.set(current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-Channel") == 0)
        {
          this->register_event_channel_ = 1;
          // Register one EC with naming.

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s -Factory factory_name "
                     "-Boot -[No]NameSvc "
                     "-IORoutput file_name "
                     "-Channel -ChannelName channel_name\n"
                     "default: %s -Factory NotifyEventChannelFactory "
                     "-NameSvc -Channel NotifyEventChannel\n",
                     argv[0], argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
            arg_shifter.ignore_arg ();
        }
    }
    return 0;
}

int
main (int argc, char *argv[])
{
  Notify_Service service;

  ACE_TRY_NEW_ENV
    {
      if (service.startup (argc,
                           argv,
                           ACE_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Failed to start the Notification Service.\n"),
                          1);
      ACE_TRY_CHECK;

      if (service.run () == -1)
        {
          service.shutdown ();
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Failed to run the Notification Service.\n"),
                            1);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Failed to start the Notification Service\n");
      return 1;
    }
  ACE_ENDTRY;

  service.shutdown ();

  return 0;
}
