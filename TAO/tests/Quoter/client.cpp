// $Id$

#include "ace/Read_Buffer.h"
#include "orbsvcs/CosNamingC.h"
#include "client.h"

ACE_RCSID(Quoter, client, "$Id$")

Quoter_Task::Quoter_Task (int argc, char **argv)
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
    useLifeCycleService_(0)  // use the Generic Factory
{
  // Nothing
}

// Parses the command line arguments and returns an error status.

int
Quoter_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "n:dlx");
  int c;
  
  while ((c = get_opts ()) != -1)
    switch (c)
    {   
      case 'n':  // multiple threads
        // ignore it, it was handled already
        break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'l':
        this->useLifeCycleService_ = 1;
        break;
      case 'x': 
        this->shutdown_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage:  %s"
                          " [-m]"
                          " [-d]"
                          " [-l] # use the lifecycle service instead of the generic factory"
                          " [-x]"
                          " [-s]"
                          "\n",
                          this->argv_ [0]),
                          -1);
    }
  
  // Indicates successful parsing of command line.
  return 0;
}

int
Quoter_Client::run (void)
{
  CORBA::Long q = this->quoter_var_->get_quote ("ACE Hardware", this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with get_quote.");
    this->env_.clear();
    return -1;
  }
  ACE_DEBUG ((LM_DEBUG, "ACE Hardware = %i\n", q));

  // Copy the Quoter

  CosLifeCycle::Criteria criteria;
  CORBA::Object_var quoterObj_var =
    this->quoter_var_->copy (factory_Finder_var_.in (), 
			     criteria,
			     this->env_);

  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with copy.");
    this->env_.clear();
    return -1;
  }

  if (CORBA::is_nil (quoterObj_var.in()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Quoter_Client::run: Copied Object pointer is nil!"),
                      -1);

  // Narrow it to the actual Quoter interface

  Stock::Quoter_var copied_quoter_var =
    Stock::Quoter::_narrow (quoterObj_var.in (),
			    this->env_);

  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with narrow.");
    this->env_.clear();
    return -1;
  }
  if (CORBA::is_nil (copied_quoter_var.in()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Quoter_Client::run: Copied Quoter is nil!"),
                      -1);
  
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Copied object.\n"));

  q = 0;
  q = copied_quoter_var->get_quote ("ACE Hardware", this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with get_quote on copied object.");
    this->env_.clear();
    return -1;
  }
  
  ACE_DEBUG ((LM_DEBUG, "Copied object: ACE Hardware = %i\n", q));

  // Move the Quoter
  this->quoter_var_->move (factory_Finder_var_.in (), 
			     criteria,
			     this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with move.");
    this->env_.clear();
    return -1;
  }

  // Caution, the object reference stays the same
    
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Moved object\n"));

  q = 0;
  q = this->quoter_var_->get_quote ("ACE Hardware", this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with get_quote on moved object.");
    this->env_.clear();
    return -1;
  }
  
  ACE_DEBUG ((LM_DEBUG, "Moved object: ACE Hardware = %i\n", q));
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
  TAO_TRY
  {
    // Resolve the Naming Service
    CORBA::Object_var naming_obj = 
      orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil (naming_obj.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        "Unable to resolve the Name Service.\n"),
         -1);

    CosNaming::NamingContext_var naming_context = 
      CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Have a proper reference to the Naming Service.\n"));

 
    CosNaming::Name quoterFactoryFinderName (2);
    quoterFactoryFinderName.length (2);
    quoterFactoryFinderName[0].id = CORBA::string_dup ("IDL_Quoter");
    quoterFactoryFinderName[1].id = CORBA::string_dup ("Quoter_Factory_Finder");

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Trying to resolve the Quoter Factory Finder!\n"));

    CORBA::Object_var factory_obj =
      naming_context->resolve (quoterFactoryFinderName,
                               TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Resolved the Quoter Factory Finder!\n"));
    
    factory_Finder_var_ =
      Stock::Quoter_Factory_Finder::_narrow (factory_obj.in (), 
                                             TAO_TRY_ENV);
    TAO_CHECK_ENV;
   
    if (CORBA::is_nil (factory_Finder_var_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " could not resolve quoter factory in Naming service <%s>\n"),
      -1);

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Have a proper reference to the Quoter Factory Finder.\n"));

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
    
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Trying to get a reference of a factory.\n"));

    // Find an appropriate factory over there.
    CosLifeCycle::Factories_ptr factories_ptr = 
        factory_Finder_var_->find_factories (factoryName, TAO_TRY_ENV);

    if (factories_ptr == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Did not get a Generic Quoter Factory.\n"),
                         -1);

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Got a proper reference of a factory.\n"));


    // Get the first object reference to a factory.
    CORBA::Object_var quoter_FactoryObj_var;

    if (factories_ptr->length () >= 1)
    {
      // everything is ok, at least one factory is there
      quoter_FactoryObj_var = (*factories_ptr)[0];
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "No Factory received.\n"),
                         -1);
    }

    // Narrow it to a Quoter Generic Factory
    generic_Factory_var_ = CosLifeCycle::GenericFactory::_narrow (quoter_FactoryObj_var.in (), TAO_TRY_ENV);

    TAO_CHECK_ENV;

    if (CORBA::is_nil (this->generic_Factory_var_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Factory received is not valid.\n"),
                         -1);

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Have a proper reference to the Quoter Factory.\n"));
  }
  TAO_CATCH (CosLifeCycle::NoFactory, excpt)
  {
    TAO_TRY_ENV.print_exception ("Quoter::init_naming_service: No Factory available!");
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
Quoter_Client::init (int argc, char **argv)
{
  this->argc_ = argc;
  int i;

  // Make a copy of argv since ORB_init will change it.
  this->argv_ = new char *[argc];
  
  for (i = 0; i < argc; i++)
    this->argv_[i] = argv[i];
  
  TAO_TRY
  {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;
    
      // deprecated
      //if (this->use_naming_service_) 
      //{
          int naming_result = this->init_naming_service ();
          if (naming_result == -1)
            return naming_result;
    
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));
    
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
						   criteria,
						   TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      this->quoter_var_ = Stock::Quoter::_narrow (quoterObject_var.in(), TAO_TRY_ENV);     
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Quoter Created\n"));
    
      if (CORBA::is_nil (this->quoter_var_.in()))
      {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "null quoter objref returned by factory\n"),
                            -1);
      }
    }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Quoter::init");
    return -1;
  }
  TAO_ENDTRY;
  
  return 0;
}


// This function runs the test.

int
main (int argc, char **argv)
{
  ACE_Thread_Manager thr_mgr;

  ACE_DEBUG ((LM_DEBUG,"\n\tQuoter: client\n\n"));

  int i;
  int threads = 1;

  for (i = 0; i < argc; i++)
    if (ACE_OS::strcmp (argv[i], "-n") == 0)
      threads = ACE_OS::atoi(argv[i + 1]);
    
  Quoter_Task **clients = new Quoter_Task*[threads];
    
  for (i = 0; i < threads; i++)
    clients[i] = new Quoter_Task (argc, argv);


  for (i = 0; i < threads; i++)
    clients[i]->activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);

  int result = ACE_Thread_Manager::instance ()->wait ();

  for (i = 0; i < threads; i++)
    delete clients[i];

  delete [] clients;

  return result;
}

