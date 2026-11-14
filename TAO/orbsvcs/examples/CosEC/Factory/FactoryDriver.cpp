// -*- C++ -*-
#include "FactoryDriver.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

FactoryDriver::FactoryDriver (ACE_TCHAR const *name)
  : factory_name_ (name == nullptr ? ACE_TEXT ("CosEC_Factory") : name)
{
}

int
FactoryDriver::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dr:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;

      case 'r':
        this->factory_name_ = get_opts.opt_arg ();
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" [-r] <Factory Name>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
FactoryDriver::start (int argc, ACE_TCHAR *argv [])
{
  try
    {
      orb_ = CORBA::ORB_init (argc, argv);

      if (this->parse_args (argc, argv) == -1)
        return -1;

      CORBA::Object_var const poa_object =
        orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Unable to initialize the POA.\n")),
                          -1);

      root_poa_ =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa_->the_POAManager ();
      poa_manager->activate ();

      // Initialization of the naming service.
      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Unable to initialize ")
                           ACE_TEXT ("the TAO_Naming_Client.\n")),
                          1);

      CosNaming::NamingContext_var const context =
        naming_client_.get_context ();

      TAO_CosEventChannelFactory_i *factory_servant {};
      ACE_NEW_RETURN (factory_servant,
                      TAO_CosEventChannelFactory_i,
                      -1);

      {
        // Keep the initial servant reference owned through POA activation.
        PortableServer::ServantBase_var const factory_servant_owner (factory_servant);

        if (factory_servant->init (root_poa_.in (),
                                   child_poa_name_,
                                   context.in ()) != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%P|%t) Unable to initialize ")
                             ACE_TEXT ("the factory.\n")),
                            1);

        // Activate the factory in the root POA.
        factory_ = factory_servant->_this ();
      }

      CORBA::String_var const str = orb_->object_to_string (factory_.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CosEvent_Service: The Cos Event Channel Factory IOR is <%C>\n"),
                  str.in ()));

      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (this->factory_name_.c_str ()));
      naming_client_->rebind (name, factory_.in ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Registered with the naming service as %s\n"),
                  this->factory_name_.c_str ()));

      orb_->run ();
    }
  catch (CORBA::UserException const &ue)
    {
      ue._tao_print_exception (ACE_TEXT ("cosecfactory: "));
      return 1;
    }
  catch (CORBA::SystemException const &se)
    {
      se._tao_print_exception (ACE_TEXT ("cosecfactory: "));
      return 1;
    }

  return 0;
}

int
FactoryDriver::stop ()
{
  orb_->shutdown ();
  return 0;
}
