// $Id$

//#include "ace/Profile_Timer.h"
//#include "ace/Env_Value_T.h"

#include "ace/Read_Buffer.h"
#include "quoter_client.h"
#include "orbsvcs/CosNamingC.h"

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

/*  Stock::Quoter_Factory_var factory_;

  TAO_TRY
  {
    CORBA::Object_var naming_obj =
	  this->orb_->resolve_initial_references ("NameService");

    if (CORBA::is_nil (naming_obj.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
                        " (%P|%t) Unable to resolve the Name Service.\n"),
                        -1);

    CosNaming::NamingContext_var naming_context = 
    CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);

    TAO_CHECK_ENV;
      
    CosNaming::Name quoter_factory_name (2);
    quoter_factory_name.length (2);
    quoter_factory_name[0].id = CORBA::string_dup ("Quoter");
    quoter_factory_name[1].id = CORBA::string_dup ("quoter_factory");

    CORBA::Object_var factory_obj =
      naming_context->resolve (quoter_factory_name, TAO_TRY_ENV);

    TAO_CHECK_ENV;
    
    Stock::Quoter_Factory_var factory_ =
      Stock::Quoter_Factory::_narrow (factory_obj.in (),
      TAO_TRY_ENV);
    
    TAO_CHECK_ENV;
    
    if (CORBA::is_nil (factory_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " could not resolve quoter factory in Naming service <%s>\n"),
      -1);

    ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));
  
    // Now retrieve the Quoter obj ref corresponding to the key.
    Stock::Quoter_ptr quoter_ =
      factory_->create_quoter (ACE_OS::strdup ("key0"), TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    if (CORBA::is_nil (quoter_))
      ACE_ERROR_RETURN ((LM_ERROR, "null quoter objref returned by factory\n"), -1);
    
    CORBA::Long q = quoter_->get_quote ("ACE Hardware", this->env_);
    if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("with get_quote");
      return -1;
    }
    else
    { 
      ACE_DEBUG ((LM_DEBUG, "ACE Hardware = %i\n", q));
      return q;
    }
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Quoter_svc");
    return -1;
  }
  TAO_ENDTRY;
  return 0;*/
}

// Constructor.
Quoter_Client::Quoter_Client (void)
  : quoter_factory_key_ (0),
    quoter_key_ (ACE_OS::strdup ("key0")),
    shutdown_ (0),
    quoter_ (Stock::Quoter::_nil ()),
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
  CORBA::Long q = this->quoter_->get_quote ("ACE Hardware", this->env_);
  if (this->env_.exception () != 0)
    {
      this->env_.print_exception ("with get_quote");
      return -1;
    }
  else
    { 
      ACE_DEBUG ((LM_DEBUG, "ACE Hardware = %i\n", q));
      return q;
    }
}

Quoter_Client::~Quoter_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->quoter_factory_ior_file_)
    ACE_OS::fclose (this->quoter_factory_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  CORBA::release (this->quoter_);

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
    CORBA::ORB_ptr orb_ptr = TAO_ORB_Core_instance ()->orb ();
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
    
    CosNaming::Name quoter_factory_name (2);
    quoter_factory_name.length (2);
    quoter_factory_name[0].id = CORBA::string_dup ("IDL_Quoter");
    quoter_factory_name[1].id = CORBA::string_dup ("quoter_factory");

    CORBA::Object_var factory_obj =
      naming_context->resolve (quoter_factory_name,
                               TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    this->factory_ =
      Stock::Quoter_Factory::_narrow (factory_obj.in (), 
                                      TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    if (CORBA::is_nil (this->factory_.in ()))
      ACE_ERROR_RETURN ((LM_ERROR,
      " could not resolve quoter factory in Naming service <%s>\n"),
      -1);
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
  int naming_result;
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
    
      if (this->use_naming_service_) 
        {
          naming_result = this->init_naming_service ();
          if (naming_result == -1)
            return naming_result;
        }
      else if (this->quoter_factory_key_ == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: no quoter factory key specified\n",
                             this->argv_[0]),
                            -1);
      
      
          CORBA::Object_var factory_object = 
            this->orb_->string_to_object (this->quoter_factory_key_,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;
      
          this->factory_ = 
            Stock::Quoter_Factory::_narrow (factory_object.in (), TAO_TRY_ENV);
          TAO_CHECK_ENV;
      
          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->quoter_factory_key_),
                              -1);
        }
    
      ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));
    
      // Now retrieve the Quoter obj ref corresponding to the key.
      this->quoter_ =
        this->factory_->create_quoter (this->quoter_key_,
                                       TAO_TRY_ENV);
    
      TAO_CHECK_ENV;
      ACE_DEBUG ((LM_DEBUG, "Quoter Created\n"));
    
      if (CORBA::is_nil (this->quoter_))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null quoter objref returned by factory\n"),
                          -1);
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Quoter::init");
      return -1;
    }
  TAO_ENDTRY;
  
  return 0;
}

/*
Quoter_Client::init (int argc, char **argv)
{
  int naming_result;
  this->argc_ = argc;
  this->argv_ = argv;
  
  TAO_TRY
  {
    // Retrieve the ORB.
    this->orb_ = CORBA::ORB_init (this->argc_, this->argv_, "internet", TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    // Parse command line and verify parameters.
    if (this->parse_args () == -1)
      return -1;
    
    if (this->use_naming_service_)
    {
      naming_result = this->init_naming_service ();
      if (naming_result < 0)
        return naming_result;
    }
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
      
      this->factory_ = 
        Stock::Quoter_Factory::_narrow (factory_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
        "invalid factory key <%s>\n",
        this->quoter_factory_key_),
        -1);
    }
    
    ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));
    
    // Now retrieve the Quoter obj ref corresponding to the key.
    this->quoter_ =
      this->factory_->create_quoter (this->quoter_key_,
      TAO_TRY_ENV);
    TAO_CHECK_ENV;
    
    if (CORBA::is_nil (this->quoter_))
      ACE_ERROR_RETURN ((LM_ERROR,
			   "null quoter objref returned by factory\n"),
         -1);
  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("Quoter::init");
    return -1;
  }
  TAO_ENDTRY;
  
  return 0;
}*/

// This function runs the test.

int
main (int argc, char **argv)
{
/*  Quoter_Client quoter_client;

  ACE_DEBUG ((LM_DEBUG,"\n \t IDL_Quoter: client \n\n"));

  if (quoter_client.init (argc, argv) == -1)
    return 1;
  else
    return quoter_client.run ();*/

  ACE_Thread_Manager thr_mgr;

  Quoter_Task client1 (argc, argv); //this->quoter_);
  Quoter_Task client2 (argc, argv); //this->quoter_);
  Quoter_Task client3 (argc, argv); //this->quoter_);

  client1.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);
//  client2.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);
//client3.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, ACE_DEFAULT_THREAD_PRIORITY);

  return ACE_Thread_Manager::instance ()->wait ();
}

