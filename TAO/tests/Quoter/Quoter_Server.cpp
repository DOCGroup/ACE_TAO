// $Id$

#include "quoter_server.h"

Quoter_Server::Quoter_Server (void)
  : num_of_objs_ (1),
    use_naming_service_ (1),
    ior_output_file_ (0)
{
}

int
Quoter_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:o:s");
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
      case 'o': // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_=0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of Quoter objects>"
                           " [-o] <ior_output_file>"
                           " [-s]"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Quoter_Server::init (int argc,
                    char** argv,
                    CORBA::Environment& env)
{
  // Call the init of TAO_ORB_Manager to create a child POA under the
  // root POA.
  this->init_child_poa (argc,
                        argv,
                        "child_poa",
                        env);

  TAO_CHECK_ENV_RETURN (env,-1);
  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();

  CORBA::String_var str  =
    this->activate_under_child_poa ("factory",
                                    &this->factory_impl_,
                                    env);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->use_naming_service_)
    return this->init_naming_service (env);

  return 0;
}

// Initialisation of Naming Service and register IDL_Quoter Context
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

    CosNaming::NamingContext_var naming_context = 
      CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    factory = this->factory_impl_._this (env);
    TAO_CHECK_ENV_RETURN (env,-1);

    
    CosNaming::Name quoter_context_name (1);
    quoter_context_name.length (1);
    quoter_context_name[0].id = CORBA::string_dup ("IDL_Quoter");

    this->quoter_context_ =
      naming_context->bind_new_context (quoter_context_name,
                                    env);
    TAO_CHECK_ENV_RETURN (env,-1);

    //Register the quoter_factory name with the IDL_quoter Naming
    //Context...
    CosNaming::Name factory_name (1);
    factory_name.length (1);
    factory_name[0].id = CORBA::string_dup ("quoter_factory");
    this->quoter_context_->bind (factory_name,
                                 factory.in (),
                                 env);
    TAO_CHECK_ENV_RETURN (env,-1);

//    CosNaming::Name quoter_factory_name (2);
//    quoter_factory_name.length (2);
//    quoter_factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
//    quoter_factory_name[1].id = CORBA::string_dup ("quoter_factory");
//    CORBA::Object_var factory_obj =
//      naming_context->resolve (quoter_factory_name,TAO_TRY_ENV);
//    TAO_CHECK_ENV;
//    
//    this->factory_ =
//      Stock::Quoter_Factory::_narrow (factory_obj.in (),TAO_TRY_ENV);
//    TAO_CHECK_ENV;
//    
//    if (CORBA::is_nil (this->factory_.in ()))
//      ACE_ERROR_RETURN ((LM_ERROR,
//      " could not resolve quoter factory in Naming service <%s>\n"),
//      -1);
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
  this->poa_manager_->activate (env);
  TAO_CHECK_ENV_RETURN (env,1);

  if (this->orb_->run () == -1)
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
      factory_name[1].id = CORBA::string_dup ("quoter_factory");
      // this->naming_context_->unbind (factory_name,TAO_TRY_ENV);
      this->my_name_server_->unbind (factory_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;
      factory_name.length (1);
      //      this->naming_context_->unbind
      //      (factory_name,TAO_TRY_ENV);
      this->my_name_server_->unbind (factory_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;
      // Destroy all the POAs.
      //      this->root_poa_->destroy (CORBA::B_TRUE,
      //                                CORBA::B_TRUE,
      //                                TAO_TRY_ENV);
      TAO_CHECK_ENV;
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
              "\n\tIDL_Quoter:SERVER \n \n"));
  TAO_TRY
    {
      if (quoter_server.init (argc,argv,TAO_TRY_ENV) == -1)
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
