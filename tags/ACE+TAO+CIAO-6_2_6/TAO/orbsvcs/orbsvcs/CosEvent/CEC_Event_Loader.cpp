// $Id$

//==========================================================================
// LIBRARY
//   libTAO_CosEvent.so
//
// DESCRIPTION
//   This class allows for dynamically loading
//   the Event Service.
//
// AUTHOR
//   Priyanka Gontla <pgontla@ece.uci.edu>
//   Carlos O'Ryan <coryan@uci.edu>
//   Jon Astle <jon@astle45.fsnet.co.uk>
//
//===========================================================================

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/CosEvent/CEC_Event_Loader.h"
#include "ace/Dynamic_Service.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CEC_Event_Loader::TAO_CEC_Event_Loader (void) :
  attributes_ (0)
  , factory_ (0)
  , ec_impl_ (0)
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  , typed_attributes_ (0)
  , typed_ec_impl_ (0)
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
{
  // Constructor
}

TAO_CEC_Event_Loader::~TAO_CEC_Event_Loader (void)
{
  // Destructor
}

int
TAO_CEC_Event_Loader::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB initialization boiler plate...
      this->orb_=
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        this->create_object (this->orb_.in (), argc, argv);

      if (CORBA::is_nil (obj.in() ))
        return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (argv[0]);
      return -1;
    }
  return 0;
}


CORBA::Object_ptr
TAO_CEC_Event_Loader::create_object (CORBA::ORB_ptr orb,
                                     int argc,
                                     ACE_TCHAR *argv[])
{
  try
    {
      // ****************************************************************

      // @@ Some ideas for improvement (not that you have to implement
      // them):
      //  - Receive a child POA name from the command line, and create
      //    all the objects in that child POA (easier to destroy stuff
      //    that way).
      //  - Receive an option to register the Event Service with the
      //    _tao_add_to_IOR_table() stuff..

      // Parse the options, check if we should bind with the naming
      // service and under what name...
      ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:o:p:xrtdb"));
      int opt;
      const ACE_TCHAR *service_name = ACE_TEXT("CosEventService");
      const ACE_TCHAR *ior_file = 0;
      const ACE_TCHAR *pid_file = 0;
      this->bind_to_naming_service_ = 1;
      int use_rebind = 0;
      int disconnect_callbacks = 0;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      // Flag to create a typed event channel
      int typed_ec = 0;
      // Flag to destroy the event channel on shutdown
      int destroy = 0;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

      while ((opt = get_opt ()) != EOF)
        {
          switch (opt)
            {
            case 'n':
              service_name = get_opt.opt_arg ();
              break;

            case 'o':
              ior_file = get_opt.opt_arg ();
              break;

            case 'p':
              pid_file = get_opt.opt_arg ();
              break;

            case 'x':
              this->bind_to_naming_service_ = 0;
              break;

            case 'r':
              use_rebind = 1;
              break;

            case 'b':
              disconnect_callbacks = 1;
              break;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
            case 't':
              typed_ec = 1;
              break;

            case 'd':
              destroy = 1;
              break;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

            case '?':
            default:
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Usage: %s ")
                          ACE_TEXT ("-n service_name ")
                          ACE_TEXT ("-o ior_file_name ")
                          ACE_TEXT ("-p pid_file_name ")
                          ACE_TEXT ("-x [disable naming service bind]")
                          ACE_TEXT ("-r [rebind, no AlreadyBound failures] ")
                          ACE_TEXT ("-b [send callBacks on disconnect] ")
                          ACE_TEXT ("-t [enable typed event channel]")
                          ACE_TEXT ("-d [destroy typed event channelon shutdown] ")
                          ACE_TEXT ("\n"),
                          argv[0]));
#else
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Usage: %s ")
                          ACE_TEXT ("-n service_name ")
                          ACE_TEXT ("-o ior_file_name ")
                          ACE_TEXT ("-p pid_file_name ")
                          ACE_TEXT ("-x [disable naming service bind] ")
                          ACE_TEXT ("-r [rebind, no AlreadyBound failures] ")
                          ACE_TEXT ("-b [send callBacks on disconnect] ")
                          ACE_TEXT ("\n"),
                          argv[0]));
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
              return CORBA::Object::_nil ();
            }
        }

      // ***************************************************************

      // POA initialization and activation ...
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in ());
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager ();
      poa_manager->activate ();


      // ****************************************************************

      // Control the event loop
      this->terminate_flag_ = 0;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      if (!typed_ec)
        {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

      // Create and activate the event service
      this->attributes_ = new TAO_CEC_EventChannel_Attributes(poa.in (),
                                                              poa.in ());

      this->attributes_->disconnect_callbacks = disconnect_callbacks;

      this->factory_ = 0;

      this->ec_impl_ = new TAO_CEC_EventChannel (*this->attributes_,
                                                 this->factory_,
                                                 this->terminate_flag_);

      this->ec_impl_->activate ();

      CosEventChannelAdmin::EventChannel_var event_channel =
        this->ec_impl_->_this ();

      if (ior_file != 0)
        {
          CORBA::String_var ior =
            orb->object_to_string (event_channel.in ());

          FILE *iorf = ACE_OS::fopen (ior_file, ACE_TEXT("w"));
          if (iorf != 0)
            {
              ACE_OS::fprintf (iorf, "%s\n", ior.in ());
              ACE_OS::fclose (iorf);
            }
        }

      if (pid_file != 0)
        {
          FILE *pidf = ACE_OS::fopen (pid_file, ACE_TEXT("w"));
          if (pidf != 0)
            {
              ACE_OS::fprintf (pidf,
                               "%ld\n",
                               static_cast<long> (ACE_OS::getpid ()));
              ACE_OS::fclose (pidf);
            }
        }

      // ****************************************************************

      this->channel_name_.length (1);

      if (this->bind_to_naming_service_)
        {
          CORBA::Object_var obj =
            orb->resolve_initial_references ("NameService");

          this->naming_context_ =
            CosNaming::NamingContext::_narrow (obj.in ());

          this->channel_name_.length (1);
          this->channel_name_[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(service_name));

          if (use_rebind)
            {
              this->naming_context_->rebind (this->channel_name_,
                                      event_channel.in ());
            }
          else
            {
              this->naming_context_->bind (this->channel_name_,
                                    event_channel.in ());
            }
        }
          return CosEventChannelAdmin::EventChannel::_duplicate (event_channel.in () );

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
        }
      else
        {
          // If Typed EC, initialize the IFR, then create and activate the typed event service

          // ****************************************************************
          // IFR initialization
          if (TAO_debug_level >= 10)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("***** Initializing the IFR connection... *****\n")));
            }

          CORBA::Repository_var interface_repository;

          CORBA::Object_var ifr_obj_var =
            orb->resolve_initial_references ("InterfaceRepository");

          if (CORBA::is_nil(ifr_obj_var.in () ))
            {
              if (TAO_debug_level >= 10)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("***** resolve_initial_references for IFR failed\n *****")));
                }
              return CORBA::Object::_nil ();
            }
          else
            {
              interface_repository = CORBA::Repository::_narrow(ifr_obj_var.in ());

              if (CORBA::is_nil(interface_repository.in () ))
                {
                  if (TAO_debug_level >= 10)
                    {
                      ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("***** CORBA::Repository::_narrow failed *****\n")));
                    }
                  return CORBA::Object::_nil ();
                }
              else
                {
                  if (TAO_debug_level >= 10)
                    {
                      ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("***** ...IFR connection completed *****\n")));
                    }
                }
            }

          // Create and activate the typed event service
          this->typed_attributes_ = new TAO_CEC_TypedEventChannel_Attributes(poa.in (),
                                                                             poa.in (),
                                                                             orb,
                                                                             interface_repository.in ());
          if (destroy == 1)
            {
              this->typed_attributes_->destroy_on_shutdown = 1;
            }

          this->typed_attributes_->disconnect_callbacks = disconnect_callbacks;

          this->factory_ = 0;

          this->typed_ec_impl_ = new TAO_CEC_TypedEventChannel (*this->typed_attributes_,
                                                                this->factory_,
                                                                this->terminate_flag_);

          this->typed_ec_impl_->activate ();

          CosTypedEventChannelAdmin::TypedEventChannel_var event_channel =
            this->typed_ec_impl_->_this ();

          if (ior_file != 0)
            {
              CORBA::String_var ior =
                orb->object_to_string (event_channel.in ());

              FILE *iorf = ACE_OS::fopen (ior_file, ACE_TEXT("w"));
              if (iorf != 0)
                {
                  ACE_OS::fprintf (iorf, "%s\n", ior.in ());
                  ACE_OS::fclose (iorf);
                }
            }

          if (pid_file != 0)
            {
              FILE *pidf = ACE_OS::fopen (pid_file, ACE_TEXT("w"));
              if (pidf != 0)
                {
                  ACE_OS::fprintf (pidf,
                                   "%ld\n",
                                   static_cast<long> (ACE_OS::getpid ()));
                  ACE_OS::fclose (pidf);
                }
            }

          // ****************************************************************

          this->channel_name_.length (1);

          if (this->bind_to_naming_service_)
            {
              CORBA::Object_var obj =
                orb->resolve_initial_references ("NameService");

              this->naming_context_ =
                CosNaming::NamingContext::_narrow (obj.in ());

              this->channel_name_.length (1);
              this->channel_name_[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(service_name));

              if (use_rebind)
                {
                  this->naming_context_->rebind (this->channel_name_,
                                                 event_channel.in ());
                }
              else
                {
                  this->naming_context_->bind (this->channel_name_,
                                               event_channel.in ());
                }
            }
          return CosTypedEventChannelAdmin::TypedEventChannel::_duplicate (event_channel.in () );
        }
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

      // ****************************************************************
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (argv[0]);
      return CORBA::Object::_nil ();
    }
}

int
TAO_CEC_Event_Loader::fini (void)
{
  //   + Since it was activated with _this() you have to do the
  //   canonical:
  //     get_object_id
  try
    {
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      if(this->typed_ec_impl_)
        {
          // Release the resources of the Typed Event Channel
          this->typed_ec_impl_->destroy ();

          // Deactivate the Typed EC
          // This will raise an exception if destroy == 1
          PortableServer::POA_var t_poa =
          this->typed_ec_impl_->_default_POA ();

          PortableServer::ObjectId_var t_id =
          t_poa->servant_to_id (this->typed_ec_impl_);

          t_poa->deactivate_object (t_id.in ());
        }
#else
      // Release the resources of the Event Channel
      this->ec_impl_->destroy ();

      // Deactivate the EC
      PortableServer::POA_var poa =
        this->ec_impl_->_default_POA ();

      PortableServer::ObjectId_var id =
        poa->servant_to_id (this->ec_impl_);

      poa->deactivate_object (id.in ());
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
    }
  catch (const CORBA::Exception&)
    {
      // Do Nothing
    }

  try
    {
      // Unbind the Naming Service
      if (this->bind_to_naming_service_)
        {
          this->naming_context_->unbind (this->channel_name_);
        }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      // Since we created them, we also have to delete them.
      delete this->typed_attributes_;
      delete this->typed_ec_impl_;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
      delete this->attributes_;
      delete this->ec_impl_;
    }
  catch (const CORBA::Exception&)
    {
      // Do Nothing
      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_Event_Serv, TAO_CEC_Event_Loader)
