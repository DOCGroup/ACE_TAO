// $Id$

#include "ace/Read_Buffer.h"
#include "orbsvcs/CosNamingC.h"
#include "client.h"

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
    quoter_factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    use_naming_service_ (1)
{
}

// Reads the Quoter factory ior from a file

int
Quoter_Client::read_ior (char *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);
  
  if (this->f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
    "Unable to open %s for writing: %p\n",
    filename),
    -1);
  ACE_Read_Buffer ior_buffer (this->f_handle_);
  this->quoter_factory_key_ = ior_buffer.read ();
  
  if (this->quoter_factory_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
   return 0;
}

// Parses the command line arguments and returns an error status.

int
Quoter_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:k:xs");
  int c;
  int result;
  
  while ((c = get_opts ()) != -1)
    switch (c)
    {   
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
          "Unable to read ior from %s : %p\n",
          get_opts.optarg),
          -1);
        break;
      case 'k': // read the quoter IOR from the command-line.
        this->quoter_factory_key_ =
          ACE_OS::strdup (get_opts.optarg);
        break;
      case 'x': 
        this->shutdown_ = 1;
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_ = 0;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage:  %s"
                          " [-d]"
                          " [-f quoter_factory-obj-ref-key-file]"
                          " [-k quoter-obj-ref-key]"
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

  CosLifeCycle::Criteria criteria;
  CORBA::Object_var quoterObj_var = this->quoter_var_->copy (factory_Finder_var_, 
                                                         criteria,
                                                         this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with copy.");
    this->env_.clear();
    return -1;
  }


  Stock::Quoter_var quoter_var =  Stock::Quoter::_narrow (quoterObj_var,
                                                          this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with narrow.");
    this->env_.clear();
    return -1;
  }
  ACE_DEBUG ((LM_DEBUG, "Copied object\n"));

  q = this->quoter_var_->get_quote ("ACE Hardware", this->env_);
  if (this->env_.exception () != 0)
  {
    this->env_.print_exception ("with get_quote on copied object.");
    this->env_.clear();
    return -1;
  }
  ACE_DEBUG ((LM_DEBUG, "Copied object: ACE Hardware = %i\n", q));

  return 0;  
}

Quoter_Client::~Quoter_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->quoter_factory_ior_file_)
    ACE_OS::fclose (this->quoter_factory_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);
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
			   " (%P|%t) Unable to resolve the Name Service.\n"),
         -1);

    CosNaming::NamingContext_var naming_context = 
      CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
    TAO_CHECK_ENV;

    ACE_DEBUG ((LM_DEBUG, "Have a proper reference to the Naming Service.\n"));

    // deprecated
    // ------------------------------------------- direct use of the Quoter Factory
    /*
    // Try to get the quoter_factory
    CosNaming::Name quoter_factory_name (2);
    quoter_factory_name.length (2);
    quoter_factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
    quoter_factory_name[1].id = CORBA::string_dup ("Quoter_Factory");

    ACE_DEBUG ((LM_DEBUG, "Trying to resolve the Quoter Factory!\n"));

    CORBA::Object_var factory_obj =
      naming_context->resolve (quoter_factory_name,
                               TAO_TRY_ENV);
    TAO_CHECK_ENV;

    ACE_DEBUG ((LM_DEBUG, "Resolved the Quoter Factory!\n"));
    
    this->factory_var_ =
      Stock::Quoter_Factory::_narrow (factory_obj.in (), 
                                      TAO_TRY_ENV);
    TAO_CHECK_ENV;
   
    if (CORBA::is_nil (this->factory_var_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " could not resolve quoter factory in Naming service <%s>\n"),
      -1);*/
    // -------------------------------------------- end of direct use

    CosNaming::Name quoterFactoryFinderName (2);
    quoterFactoryFinderName.length (2);
    quoterFactoryFinderName[0].id = CORBA::string_dup ("IDL_Quoter");
    quoterFactoryFinderName[1].id = CORBA::string_dup ("Quoter_Factory_Finder");

    ACE_DEBUG ((LM_DEBUG, "Trying to resolve the Quoter Factory Finder!\n"));

    CORBA::Object_var factory_obj =
      naming_context->resolve (quoterFactoryFinderName,
                               TAO_TRY_ENV);
    TAO_CHECK_ENV;

    ACE_DEBUG ((LM_DEBUG, "Resolved the Quoter Factory Finder!\n"));
    
    factory_Finder_var_ =
      Stock::Quoter_Factory_Finder::_narrow (factory_obj.in (), 
                                             TAO_TRY_ENV);
    TAO_CHECK_ENV;
   
    if (CORBA::is_nil (factory_Finder_var_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " could not resolve quoter factory in Naming service <%s>\n"),
      -1);

    ACE_DEBUG ((LM_DEBUG, "Have a proper reference to the Quoter Factory Finder.\n"));

    // The name of the Quoter Factory
    CosLifeCycle::Key factoryName (1);  // max = 1 
    factoryName.length(1);

    // to use directly a Quoter_Generic_Factory
    // factoryName[0].id = CORBA::string_dup ("Quoter_Generic_Factory");
    // or to use the life cycle service
    factoryName[0].id = CORBA::string_dup ("Quoter_Life_Cycle_Service");
    
    // Find an appropriate factory over there.
    CosLifeCycle::Factories_ptr factories_ptr = 
        factory_Finder_var_->find_factories (factoryName, TAO_TRY_ENV);

    if (factories_ptr == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Did not get a Generic Quoter Factory.\n"),
                         -1);

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

    // Narrow it
    // to a direct Quoter Factory
    // factory_var_ = Stock::Quoter_Factory::_narrow (quoter_FactoryObj_var.in (), TAO_TRY_ENV);
    // to a Quoter Generic Factory
    generic_Factory_var_ = Stock::Quoter_Generic_Factory::_narrow (quoter_FactoryObj_var.in (), TAO_TRY_ENV);

    TAO_CHECK_ENV;

    if (CORBA::is_nil (this->generic_Factory_var_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Factory received is not valid.\n"),
                         -1);

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
  this->argv_ = argv;
  
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
      /*}
      else
          {
          if (this->quoter_factory_key_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: no quoter factory key specified\n",
                              this->argv_[0]),
                              -1);

          CORBA::Object_var factory_object = 
            this->orb_->string_to_object (this->quoter_factory_key_,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;
      
          this->factory_var_ = Stock::Quoter_Factory::_narrow (factory_object.in (), 
                                                           TAO_TRY_ENV);
          TAO_CHECK_ENV;
      
          if (CORBA::is_nil (this->factory_var_.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,"invalid factory key <%s>\n",
                                 this->quoter_factory_key_),
                                -1);
            }
        }*/
    
      ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));
    
      // Now retrieve the Quoter obj ref corresponding to the key.
      // direct Quoter Factory

      // this->quoter_ = this->factory_var_->create_quoter (this->quoter_key_, TAO_TRY_ENV);

      // using the Quoter Generic Factory
      CosLifeCycle::Key genericFactoryName (1);  // max = 1 
      genericFactoryName.length(1);
      genericFactoryName[0].id = CORBA::string_dup ("Generic_Quoter_Factory");
      CORBA::Object_var quoterObject_var = 
          this->generic_Factory_var_->create_object (genericFactoryName,
                                                     0, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->quoter_var_ = Stock::Quoter::_narrow (quoterObject_var.in(), TAO_TRY_ENV);     
      TAO_CHECK_ENV;

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
//  Quoter_Client quoter_client;
//
//  ACE_DEBUG ((LM_DEBUG,"\n \t IDL_Quoter: client \n\n"));
//
//  if (quoter_client.init (argc, argv) == -1)
//    return 1;
//  else
//    return quoter_client.run ();

  ACE_Thread_Manager thr_mgr;

  Quoter_Task client1 (argc, argv);
//  Quoter_Task client2 (argc, argv);
//  Quoter_Task client3 (argc, argv);

  client1.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);
//  client2.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);
//  client3.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);

  return ACE_Thread_Manager::instance ()->wait ();
}

