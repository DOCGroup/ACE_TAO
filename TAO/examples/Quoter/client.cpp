#include "ace/Read_Buffer.h"

#include "orbsvcs/CosNamingC.h"

#include "client.h"

#include <memory>
#include <new>
#include <vector>

Quoter_Task::Quoter_Task (int argc, ACE_TCHAR **argv)
  : argc_ (argc), argv_ (argv)
{
  // Nothing
}

int
Quoter_Task::svc ()
{
  if (this->quoter_client.init (this->argc_, this->argv_) == -1)
    return 1;
  else
    return this->quoter_client.run ();
}

// Parses the command line arguments and returns an error status.

int
Quoter_Client::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, this->argv_.get (), ACE_TEXT("n:d:lx"));
  int opt;
  int exit_code {};

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
        this->useLifeCycleService_ = true;
        break;
      case 'x':
        this->shutdown_ = true;
        break;
      default:
        exit_code = 1;
        ACE_ERROR ((LM_ERROR,
                    "%s: unknown arg, -%c\n",
                    this->argv_[0], static_cast<char> (opt)));
        ACE_FALLTHROUGH;
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
Quoter_Client::run ()
{
  if (this->debug_level_ >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "\nQuoter Example: Quoter_Client is running\n"));

  char const *exception_message = "Null Message";
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
  catch (CORBA::Exception const &ex)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Client::run - %C\n", exception_message));
      ex._tao_print_exception ("Quoter_Client::run");
      return -1;
    }

  return 0;
}

int
Quoter_Client::init_naming_service ()
{
  char const *exception_message = "Null Message";

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

      if (this->useLifeCycleService_)
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
      CosLifeCycle::Factories_var factories =
          factory_Finder_var_->find_factories (factoryName);

      if (factories.ptr () == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Did not get a Generic Quoter Factory.\n"),
                          -1);

      if (this->debug_level_ >= 2)
        ACE_DEBUG ((LM_DEBUG, "Quoter Client: Got a proper reference of a factory.\n"));

      // Get the first object reference to a factory.
      CORBA::Object_var quoter_FactoryObj_var;

      if (factories->length () >= 1)
        {
          // everything is ok, at least one factory is there
          CosLifeCycle::Factory_ptr factory = (*factories)[0];
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
  catch (CosLifeCycle::NoFactory const &excpt)
    {
      ACE_ERROR ((LM_ERROR, "Quoter_Client::run - %C\n", exception_message));
      excpt._tao_print_exception (
        "Quoter::init_naming_service: No Factory available!");
    }
  catch (CORBA::Exception const &ex)
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
  // Make a copy of argv since ORB_init will change it.
  ACE_TCHAR **argv_copy {};
  ACE_NEW_RETURN (argv_copy, ACE_TCHAR *[argc], -1);
  this->argv_.reset (argv_copy);

  for (int i = 0; i < argc; i++)
    this->argv_[i] = argv[i];

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_.get (),
                                    "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      int const naming_result = this->init_naming_service ();
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
  catch (CORBA::Exception const &ex)
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
  int threads = 1;

  for (int i = 0; i < argc; i++)
    if (argv[i] != nullptr && ACE_OS::strcmp (argv[i], ACE_TEXT("-n")) == 0)
      {
        if (i + 1 >= argc)
          ACE_ERROR_RETURN ((LM_ERROR, "Missing thread count after -n\n"), -1);
        threads = ACE_OS::atoi (argv[i + 1]);
      }

  if (threads <= 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Thread count must be positive\n"), -1);

  std::vector<std::unique_ptr<Quoter_Task>> clients;
  try
    {
      clients.reserve (threads);
      for (int i = 0; i < threads; i++)
        clients.emplace_back (std::make_unique<Quoter_Task> (argc, argv));
    }
  catch (std::bad_alloc const&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate Quoter client tasks\n"), -1);
    }

  for (std::unique_ptr<Quoter_Task> const& client : clients)
    client->activate (THR_BOUND | THR_SCHED_FIFO, 1, 0,
                      ACE_DEFAULT_THREAD_PRIORITY);

  int const result = ACE_Thread_Manager::instance ()->wait ();

  return result;
}
