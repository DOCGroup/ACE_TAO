// $ Id$

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
//
//===========================================================================

#include "CEC_Event_Loader.h"
#include "ace/Dynamic_Service.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"

#include "ace/Get_Opt.h"

ACE_RCSID (CosEvent, Event_Loader, "$Id$")

TAO_CEC_Event_Loader::TAO_CEC_Event_Loader (void)
{
  // Constructor
}

TAO_CEC_Event_Loader::~TAO_CEC_Event_Loader (void)
{
  // Destructor
}

int
TAO_CEC_Event_Loader::init (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      this->orb_=
        CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        this->create_object (this->orb_.in (), argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, argv[0]);
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}


CORBA::Object_ptr
TAO_CEC_Event_Loader::create_object (CORBA::ORB_ptr orb,
                                     int argc, char *argv[]
                                     ACE_ENV_ARG_DECL)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
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
      ACE_Get_Opt get_opt (argc, argv, "o:n:xr");
      int opt;
      const char *service_name = "CosEventService";
      const char *ior_file = 0;
      this->bind_to_naming_service_ = 1;
      int use_rebind = 0;

      while ((opt = get_opt ()) != EOF)
        {
          switch (opt)
            {
            case 'o':
              ior_file = get_opt.opt_arg ();
              break;

            case 'n':
              service_name = get_opt.opt_arg ();
              break;

            case 'x':
              this->bind_to_naming_service_ = 0;
              break;

            case 'r':
              use_rebind = 1;
              break;

            case '?':
            default:
              ACE_DEBUG ((LM_DEBUG,
                          "Usage: %s "
                          "-n service_name "
                          "-x [disable naming service bind] "
                          "-r [rebind, no AlreadyBound failures] "
                          "\n",
                          argv[0]));
              return CORBA::Object::_nil ();
            }
        }

      // ***************************************************************

      // POA initialization and activation ...
      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // ****************************************************************

      // Control the event loop
      this->terminate_flag_ = 0;

      // Create and activate the event service
      this->attributes_ = new TAO_CEC_EventChannel_Attributes(poa.in (),
                                                              poa.in ());
      this->factory_ = 0;

      this->ec_impl_ = new TAO_CEC_EventChannel (*this->attributes_,
                                                 this->factory_,
                                                 this->terminate_flag_);

      this->ec_impl_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CosEventChannelAdmin::EventChannel_var event_channel =
        this->ec_impl_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ior_file != 0)
        {
          CORBA::String_var ior =
            orb->object_to_string (event_channel.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          FILE *file = ACE_OS::fopen (ior_file, "w");
          ACE_OS::fprintf (file, "%s\n", ior.in ());
          ACE_OS::fclose (file);
        }

      // ****************************************************************

      this->channel_name_.length (1);

      if (this->bind_to_naming_service_)
        {
          CORBA::Object_var obj =
            orb->resolve_initial_references ("NameService"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->naming_context_ =
            CosNaming::NamingContext::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->channel_name_.length (1);
          this->channel_name_[0].id = CORBA::string_dup (service_name);

          if (use_rebind)
            {
              this->naming_context_->rebind (this->channel_name_,
                                      event_channel.in ()
                                      ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          else
            {
              this->naming_context_->bind (this->channel_name_,
                                    event_channel.in ()
                                    ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, argv[0]);
      return CORBA::Object::_nil ();
    }
  ACE_ENDTRY;

  return 0;
}

int
TAO_CEC_Event_Loader::fini (void)
{
  //   + Since it was activated with _this() you have to do the
  //   canonical:
  //     get_object_id
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Release the resources of the Event Channel
      this->ec_impl_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Deactivate the EC
      PortableServer::POA_var poa =
        this->ec_impl_->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        poa->servant_to_id (this->ec_impl_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Unbind the Naming Service
      if (this->bind_to_naming_service_)
        {
          this->naming_context_->unbind (this->channel_name_
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Since we created them, we also have to delete them.
      delete this->attributes_;
      delete this->ec_impl_;

    }
  ACE_CATCHANY
    {
      // Do Nothing
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

ACE_FACTORY_DEFINE (TAO_Event, TAO_CEC_Event_Loader)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Dynamic_Service<TAO_CEC_Event_Loader>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Dynamic_Service<TAO_CEC_Event_Loader>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
