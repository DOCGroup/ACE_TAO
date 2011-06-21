//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  The Server for the Quoter Factory
 *
 *
 *  @author Darrell Brunsch (brunsch@cs.wustl.edu) Michael Kircher (mk1@cs.wustl.edu)
 */
//=============================================================================


#include "server.h"
#include "tao/ORB_Core.h"

Quoter_Server::Quoter_Server (void)
  : num_of_objs_ (1),
    quoter_Factory_i_ptr_ (0),
    debug_level_ (1)
{
}

int
Quoter_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("d:n:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'n': // number of Quoter objects we export
        this->num_of_objs_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d] <debug level>"
                           " [-n] <num of Quoter objects>"
                           "\n",
                           argv_ [0]),
                          1);
      }


  // Indicates successful parsing of command line.
  return 0;
}


//Initialize the Quoter Server

int
Quoter_Server::init (int argc,
                    ACE_TCHAR* argv[])
{
  const char *exception_message = "Null Message";
  try
    {
      exception_message = "While ORB Manager init";
      int result = this->orb_manager_.init (argc, argv);

      if (result == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "init"),
                          -1);

      // Copy them, because parse_args expects them there.
      this->argc_ = argc;
      int i;

      // Make a copy of argv since ORB_init will change it.
      this->argv_ = new ACE_TCHAR *[argc];

      for (i = 0; i < argc; i++)
        this->argv_[i] = argv[i];

      this->parse_args ();

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        this->orb_manager_.orb()->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      exception_message = "While narrowing the root pos";
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      exception_message = "While getting the POA Manager";
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      ACE_NEW_RETURN (quoter_Factory_i_ptr_,
                      Quoter_Factory_i(this->num_of_objs_,
                                       root_poa.in()),
                      0);

      exception_message = "While initing the quoter factory";
      quoter_Factory_i_ptr_->init ();

      PortableServer::ObjectId_var quoter_Factory_oid =
        PortableServer::string_to_ObjectId ("Quoter_Factory");

      exception_message = "While activating quoter factory";
      root_poa->activate_object_with_id (quoter_Factory_oid.in (),
                                         quoter_Factory_i_ptr_);

      // Get Object reference for first_foo_impl object.
      exception_message = "While quoter_Factor::_this";
      Stock::Quoter_Factory_var quoter_Factory_var = quoter_Factory_i_ptr_->_this ();

      // Stringify the object reference and print it out.
      exception_message = "While object_to_string";
      CORBA::String_var quoter_Factory_ior =
        this->orb_manager_.orb()->object_to_string (quoter_Factory_var.in ());

      exception_message = "While activating the POA Manager";
      poa_manager->activate ();

      // Print the IOR.
      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Server: IOR is: <%C>\n", quoter_Factory_ior.in ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quote_Server::init - %C\n", exception_message));
      ex._tao_print_exception ("Exception");
      throw;
    }

  return this->init_naming_service ();
}


// Initialization of Naming Service and register IDL_Quoter Context
// and Quoter_factory object.

int
Quoter_Server::init_naming_service (void)
{
  const char *exception_message = "Null Message";
  try
    {
      CORBA::ORB_ptr orb_ptr = TAO_ORB_Core_instance()->orb();

      CORBA::Object_var naming_obj =
        orb_ptr->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                                       " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);

      exception_message = "While narrowing naming context";
      namingContext_var_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name quoterContextName (1);
      quoterContextName.length (1);
      quoterContextName[0].id = CORBA::string_dup ("IDL_Quoter");

      exception_message = "While binding a new context";
      CosNaming::NamingContext_var quoterNameContext =
        namingContext_var_->bind_new_context (quoterContextName);

      //Register the quoter_factory name with the IDL_quoter Naming
      //Context...
      CosNaming::Name quoterFactoryContextName (1);
      quoterFactoryContextName.length (1);
      quoterFactoryContextName[0].id = CORBA::string_dup ("Quoter_Factory");

      exception_message = "While using factory _this";
      Stock::Quoter_Factory_var quoter_factory_var = quoter_Factory_i_ptr_->_this ();

      exception_message = "While binding factory";
      quoterNameContext->bind (quoterFactoryContextName,
                               quoter_factory_var.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quote_Server::init_naming_service - %C", exception_message));
      ex._tao_print_exception ("Exception");
      return -1;
    }

  return 0;
}

int
Quoter_Server::run (void)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Server is running\n"));

  orb_manager_.orb()->run ();

  return 0;
}

Quoter_Server::~Quoter_Server (void)
{
  try
    {
      // Unbind quoter factory context and name.
      CosNaming::Name factory_name (2);
      factory_name.length (2);
      factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
      factory_name[1].id = CORBA::string_dup ("Quoter_Factory");
      if (!CORBA::is_nil (this->namingContext_var_.in ()))
        this->namingContext_var_->unbind (factory_name);

      factory_name.length (1);
      if (!CORBA::is_nil (this->namingContext_var_.in ()))
        this->namingContext_var_->unbind (factory_name);
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "Could not unbind Quoter Server from the Name Service\n"));
      ex._tao_print_exception ("~Quoter_Server");
    }

  delete [] this->argv_;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Quoter_Server quoter_server;

  try
    {
      int result = quoter_server.init (argc, argv);

      if (result == -1)
        return 1;

      quoter_server.run ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}
