// $Id$

#include "ace/Read_Buffer.h"

#include "orbsvcs/CosNamingC.h"

#include "client.h"

Quoter_Task::Quoter_Task (int argc, ACE_TCHAR **argv)
  : argc_ (argc), argv_ (argv)
{
  // Nothing
}

int
Quoter_Task::svc (void)
{
  if (this->quoter_client.init (this->argc_, this->argv_) == -1)
    return 1;
  else
    return this->quoter_client.run ();
}

// Constructor.
Quoter_Client::Quoter_Client (void)
  : quoter_factory_key_ (0),
    quoter_key_ (ACE_OS::strdup ("key0")),
    shutdown_ (0),
    quoter_var_ (Stock::Quoter::_nil ()),
    useLifeCycleService_(0),  // use the Generic Factory
    debug_level_ (1)
{
  // Nothing
}

// Parses the command line arguments and returns an error status.

int
Quoter_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("n:d:lx"));
  int opt;
  int exit_code = 0;

  while ((opt = get_opts ()) != -1)
    switch (opt)
    {
      case 'n':  // multiple threads
        // ignore it, it was handled already
        break;
      case 'd':  // debug flag.
        this->debug_level_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'l':
        this->useLifeCycleService_ = 1;
        break;
      case 'x':
        this->shutdown_ = 1;
        break;
      default:
        exit_code = 1;
        ACE_ERROR ((LM_ERROR,
                    "%s: unknown arg, -%c\n",
                    this->argv_[0], char(opt)));
      case '?':
        ACE_DEBUG ((LM_DEBUG,
                    "usage:  %s"
                    " [-m]"
                    " [-d] <debug level> - Set the debug level\n"
                    " [-l]               - use the lifecycle service instead of the generic factory"
                    " [-x]"
                    "\n",
                    this->argv_ [0]));
        ACE_OS::exit (exit_code);
  }

  // Indicates successful parsing of command line.
  return 0;
}

int
Quoter_Client::run (void)
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Client is running\n"));

  const char *exception_message = "Null Message";
  try
    {
      exception_message = "While using get_quote ()";
      CORBA::Long q = this->quoter_var_->get_quote ("ACE Hardware");

      if (this->debug_level_ >= 1)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: ACE Hardware = %i\n", q));

      // Copy the Quoter

      CosLifeCycle::Criteria criteria;
      exception_message = "While copying the quoter";
      CORBA::Object_var quoterObj_var =
        this->quoter_var_->copy (factory_Finder_var_.in (),
                                 criteria);

      if (CORBA::is_nil (quoterObj_var.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Quoter_Client::run: Copied Object pointer is nil!"),
                          -1);

      // Narrow it to the actual Quoter interface
      exception_message = "While narrowing the quoter";
      Stock::Quoter_var copied_quoter_var =
        Stock::Quoter::_narrow (quoterObj_var.in ());

      if (CORBA::is_nil (copied_quoter_var.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Quoter_Client::run: Copied Quoter is nil!"),
                          -1);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Copied object.\n"));

      exception_message = "While using get_quote () on copied object";
      q = copied_quoter_var->get_quote ("ACE Hardware");

      if (this->debug_level_ >= 1)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Copied object: ACE Hardware = %i\n", q));

    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Client::run - %C\n", exception_message));
      ex._tao_print_exception ("Quoter_Client::run");
      return -1;
    }

  return 0;
}

Quoter_Client::~Quoter_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->quoter_factory_key_ != 0)
    ACE_OS::free (this->quoter_factory_key_);
  if (this->quoter_key_ != 0)
    ACE_OS::free (this->quoter_key_);
}

int
Quoter_Client::init_naming_service (void)
{
  const char *exception_message = "Null Message";

  try
    {
      // Resolve the Naming Service
      CORBA::Object_var naming_obj =
        orb_->resolve_initial_references ("NameService");

      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to resolve the Name Service.\n"),
                          -1);

      exception_message = "While narrowing the naming context";
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Have a proper reference to the Naming Service.\n"));

      CosNaming::Name quoterFactoryFinderName (2);
      quoterFactoryFinderName.length (2);
      quoterFactoryFinderName[0].id = CORBA::string_dup ("IDL_Quoter");
      quoterFactoryFinderName[1].id = CORBA::string_dup ("Quoter_Factory_Finder");

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Trying to resolve the Quoter Factory Finder!\n"));

      exception_message = "While resolving the factory finder";
      CORBA::Object_var factory_obj =
        naming_context->resolve (quoterFactoryFinderName);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Resolved the Quoter Factory Finder!\n"));

      exception_message = "While narrowing the factory finder";
      factory_Finder_var_ =
        Stock::Quoter_Factory_Finder::_narrow (factory_obj.in ());

      if (CORBA::is_nil (factory_Finder_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve quoter factory in Naming service\n"),
                          -1);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Have a proper reference to the Quoter Factory Finder.\n"));

      // The name of the Quoter Generic Factory
      CosLifeCycle::Key factoryName (2);  // max = 2

      if (this->useLifeCycleService_ == 1)
      {
        // use the LifeCycle Service
        factoryName.length(1);
        factoryName[0].id = CORBA::string_dup ("Life_Cycle_Service");
      }
      else
      {
        // use a Generic Factory
        factoryName.length(2);
        factoryName[0].id = CORBA::string_dup ("IDL_Quoter");
        factoryName[1].id = CORBA::string_dup ("Quoter_Generic_Factory");
      }

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Trying to get a reference of a factory.\n"));

      // Find an appropriate factory over there.
      exception_message = "While finding factories";
      CosLifeCycle::Factories *factories_ptr =
          factory_Finder_var_->find_factories (factoryName);

      if (factories_ptr == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Did not get a Generic Quoter Factory.\n"),
                          -1);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Got a proper reference of a factory.\n"));


      // Get the first object reference to a factory.
      CORBA::Object_var quoter_FactoryObj_var;

      if (factories_ptr->length () >= 1)
        {
          // everything is ok, at least one factory is there
          CosLifeCycle::Factory_ptr factory = (*factories_ptr)[0];
          quoter_FactoryObj_var = factory;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "No Factory received.\n"),
                          -1);

      // Narrow it to a Quoter Generic Factory
      exception_message = "While narrowing the factory";
      generic_Factory_var_ =
        CosLifeCycle::GenericFactory::_narrow (quoter_FactoryObj_var.in ());

      if (CORBA::is_nil (this->generic_Factory_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Factory received is not valid.\n"),
                          -1);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Have a proper reference to the Quoter Factory.\n"));
    }
  catch (const CosLifeCycle::NoFactory& excpt)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Client::run - %C\n", exception_message));
      excpt._tao_print_exception (
        "Quoter::init_naming_service: No Factory available!");
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Client::init_naming_service - %C\n", exception_message));
      ex._tao_print_exception ("Quoter::init_naming_service");
      return -1;
    }

  return 0;
}

int
Quoter_Client::init (int argc, ACE_TCHAR **argv)
{
  this->argc_ = argc;
  int i;

  // Make a copy of argv since ORB_init will change it.
  this->argv_ = new ACE_TCHAR *[argc];

  for (i = 0; i < argc; i++)
    this->argv_[i] = argv[i];

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      int naming_result = this->init_naming_service ();
      if (naming_result == -1)
        return naming_result;

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Factory received OK\n"));

      // using the Quoter Generic Factory
      CosLifeCycle::Key genericFactoryName (1);  // max = 1
      genericFactoryName.length(1);
      genericFactoryName[0].id = CORBA::string_dup ("Quoter_Factory");

      // The final factory

      CosLifeCycle::Criteria criteria(1);
      criteria.length (1);
      criteria[0].name = CORBA::string_dup ("filter");
      criteria[0].value <<= CORBA::string_dup ("name=='Quoter_Generic_Factory'");
      // used to find the last generic factory in the chain

      CORBA::Object_var quoterObject_var =
        this->generic_Factory_var_->create_object (genericFactoryName,
                                                   criteria);

      this->quoter_var_ = Stock::Quoter::_narrow (quoterObject_var.in());

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Quoter Created\n"));

      if (CORBA::is_nil (this->quoter_var_.in()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "null quoter objref returned by factory\n"),
                            -1);
      }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Quoter::init");
      return -1;
    }

  return 0;
}


// This function runs the test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_Thread_Manager thr_mgr;

  int i;
  int threads = 1;

  for (i = 0; i < argc; i++)
    if (ACE_OS::strcmp (argv[i], ACE_TEXT("-n")) == 0)
      threads = ACE_OS::atoi(argv[i + 1]);

  Quoter_Task **clients = new Quoter_Task*[threads];

  for (i = 0; i < threads; i++)
    clients[i] = new Quoter_Task (argc, argv);


  for (i = 0; i < threads; i++)
    clients[i]->activate (THR_BOUND | THR_SCHED_FIFO, 1, 0,
                          ACE_DEFAULT_THREAD_PRIORITY);

  int result = ACE_Thread_Manager::instance ()->wait ();

  for (i = 0; i < threads; i++)
    delete clients[i];

  delete [] clients;

  return result;
}
