// $Id$
// ============================================================================
//
// = FILENAME
//    Quoter_Server.cpp
//
// = DESCRIPTION
//    The Server for the Quoter Factory 
//
// = AUTHOR
//    Darrell Brunsch (brunsch@cs.wustl.edu)
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "server.h"

Quoter_Server::Quoter_Server (void)
  : num_of_objs_ (1),
    quoter_Factory_Impl_ (0)
{
}

int
Quoter_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'n': // number of Quoter objects we export
        this->num_of_objs_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of Quoter objects>"
                           "\n",
                           argv_ [0]),
                          1);
      }


  ACE_NEW_RETURN (quoter_Factory_Impl_, Quoter_Factory_Impl(this->num_of_objs_), 0);

  // Indicates successful parsing of command line.
  return 0;
}

int
Quoter_Server::init (int argc,
                    char* argv[],
                    CORBA::Environment& env)
{
  if (this->orb_manager_.init (argc,
                               argv,
                               env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       "%p\n", 
                       "init"),
                      -1);

  // Copy them, because parse_args expects them there.
  this->argc_ = argc;
  int i;

  // Make a copy of argv since ORB_init will change it.
  this->argv_ = new char *[argc];
  
  for (i = 0; i < argc; i++)
    this->argv_[i] = argv[i];

  this->parse_args ();

  // Activate the object.
  CORBA::String_var str  =
    this->orb_manager_.activate (this->quoter_Factory_Impl_,
                                 env);

  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  return this->init_naming_service (env);
}

// Initialization of Naming Service and register IDL_Quoter Context
// and Quoter_factory object.

int
Quoter_Server::init_naming_service (CORBA::Environment& env)
{
  TAO_TRY 
  {
    CORBA::ORB_ptr orb_ptr = TAO_ORB_Core_instance()->orb();
    TAO_CHECK_ENV;

    CORBA::Object_var naming_obj = 
      orb_ptr->resolve_initial_references ("NameService");

    if (CORBA::is_nil (naming_obj.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to resolve the Name Service.\n"),
         -1);

    namingContext_var_ = 
      CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
  
    CosNaming::Name quoterContextName (1);
    quoterContextName.length (1);
    quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");
    CosNaming::NamingContext_var quoterNameContext =
      namingContext_var_->bind_new_context (quoterContextName, env);
    TAO_CHECK_ENV_RETURN (env, -1);

    //Register the quoter_factory name with the IDL_quoter Naming
    //Context...
    CosNaming::Name quoterFactoryContextName (1);
    quoterFactoryContextName.length (1);
    quoterFactoryContextName[0].id = CORBA::string_dup ("Quoter_Factory");
    quoterNameContext->bind (quoterFactoryContextName,
                             quoter_Factory_Impl_->_this(env),
                             env);
    TAO_CHECK_ENV_RETURN (env, -1);

  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Quoter::init_naming_service");
    return -1;
  }
  TAO_ENDTRY;
  
  return 0;
}

int
Quoter_Server::run (CORBA::Environment& env)
{
  if (orb_manager_.orb()->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "run"),
                      -1);
  return 0;
}

Quoter_Server::~Quoter_Server (void)
{
  TAO_TRY
    {
      // Unbind quoter factory context and name.
      CosNaming::Name factory_name (2);
      factory_name.length (2);
      factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
      factory_name[1].id = CORBA::string_dup ("Quoter_Factory");
      this->namingContext_var_->unbind (factory_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;
      factory_name.length (1);
      this->namingContext_var_->unbind (factory_name,TAO_TRY_ENV);
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
    }
  TAO_ENDTRY;
}

int
main (int argc, char *argv[])
{
  Quoter_Server quoter_server;

  ACE_DEBUG ((LM_DEBUG,
              "\n\tQuoter:SERVER \n \n"));
  TAO_TRY
    {
      if (quoter_server.init (argc, argv, TAO_TRY_ENV) == -1)
        return 1;
      else
        {
          quoter_server.run (TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      ACE_UNUSED_ARG (sysex);
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      ACE_UNUSED_ARG (userex);
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
