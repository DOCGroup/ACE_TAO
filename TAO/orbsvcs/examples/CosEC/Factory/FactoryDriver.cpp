// -*- C++ -*-
// $Id$

#include "FactoryDriver.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"

FactoryDriver::FactoryDriver (const char* name)
  :factoryName_ (name),
   child_poa_name_ ("CosEC_ChildPOA"),
   factory_servant_ (0)
{
  //No-Op.
}

FactoryDriver::~FactoryDriver (void)
{
  //No-Op.
}

int
FactoryDriver::parse_args (int argc, char *argv [])
{
  ACE_Get_Opt get_opts (argc, argv, "dr:");
  int c = 0;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;

      case 'r':
        factoryName_ = get_opts.opt_arg ();
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-r] <Factory Name>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
FactoryDriver::start (int argc, char *argv [])
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              ""
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->parse_args (argc, argv) == -1)
        return -1;

      // Ref counted servants are on the heap..
      ACE_NEW_RETURN (factory_servant_,
                      TAO_CosEventChannelFactory_i (),
                      -1);

      CORBA::Object_var poa_object  =
        orb_->resolve_initial_references("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          -1);

      root_poa_ =
        PortableServer::POA::_narrow (poa_object.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa_->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;

      // Initialization of the naming service.
      if (naming_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          1);

      CosNaming::NamingContext_var context =
        naming_client_.get_context ();

      if (factory_servant_->init (root_poa_.in (),
                                  child_poa_name_,
                                  context.in ()
                                 TAO_ENV_ARG_PARAMETER) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to initialize "
                           "the factory. \n"),
                          1);

      // activate the factory in the root poa.
      factory_ = factory_servant_->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Give the ownership to the POA.
      factory_servant_->_remove_ref (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;
      CORBA::String_var
        str = orb_->object_to_string (factory_.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "CosEvent_Service: The Cos Event Channel Factory IOR is <%s>\n",
                  str.in ()));

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (factoryName_);
      naming_client_->rebind (name,
                              factory_.in ()
                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Registered with the naming service as %s\n", factoryName_));

      orb_->run ();
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "cosecfactory: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "cosecfactory: ");
      return 1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (1);

  return 0;
}

int
FactoryDriver::stop (void)
{
  orb_->shutdown ();
  return 0;
}
