// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    Start one server thread calling a distant MT Object serveral times,
//    also starting serveral client threads which call the MT Object too.
//    The server does nested upcalls.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "client.h"
#include "local_server.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(MT_Client_Test, client, "$Id$")

MT_Client_Task::MT_Client_Task (int argc, char **argv,
                                int client_number)
  : argc_ (argc),
    argv_ (argv),
    client_number_ (client_number)
{
}

int
MT_Client_Task::svc (void)
{
  if (this->mT_Client_.init (this->argc_,
                             this->argv_,
                             this->client_number_) == -1)
    return 1;
  else
    return this->mT_Client_.run ();
}

// Constructor.
MT_Client::MT_Client ()
  : object_key_ (0),
    iterations_ (1)
{
}


// Reads the Object A IOR from a file

int
MT_Client::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename,0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);

  this->object_key_ = ior_buffer.read ();
  if (this->object_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                       -1);

  ACE_OS::close (f_handle);
  return 0;
}


// Parses the command line arguments and returns an error status.

int
MT_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:g:h:i:n:s:");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
        // Depending on the thread ID we pick the IOR
      case 'f': // read the IOR from the file.
        if ((this->client_number_ % 2) == 0)
        {
          result = this->read_ior (get_opts.opt_arg ());
          // read IOR for MT Object
          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to read ior from %s : %p\n",
                               get_opts.opt_arg ()),
                              -1);
        }
        break;
      case 'g': // read the IOR from the file.
        if ((this->client_number_ % 2) == 1)
        {
          result = this->read_ior (get_opts.opt_arg ());
          // read IOR for Object A
          if (result < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to read ior from %s : %p\n",
                               get_opts.opt_arg ()),
                              -1);
        }
        break;
      case 'i': this->iterations_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
      case 'n':
      case 's':
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                          "usage:  %s\n"
                           " [-f] first server ior file\n"
                           " [-g] second server ior file\n"
                           " [-h] third server ior file\n"
                           " [-i] client iterations\n"
                           " [-n] number of client threads\n"
                           " [-s] number of server iterations\n",
                          this->argv_ [0]),
                          -1);
    }

  // Indicates successful parsing of command line.
  return 0;
}

int
MT_Client::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (unsigned long i = 0; i < this->iterations_; i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) MT_Client::run: %d of %d\n",
                      i,
                      this->iterations_));

          // call the recursive object MT_Object for nested upcalls
          // testing
          this->mT_Object_var_->yadda (0,
                                       0
                                       ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "MT_Client:run");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

MT_Client::~MT_Client (void)
{
  if (this->object_key_ != 0)
    ACE_OS::free (this->object_key_);
  if (this->argv_ != 0)
    delete [] this->argv_;
}


int
MT_Client::init (int argc, char **argv,
                 int client_number)
{

  // Make a copy of argv since ORB_init will change it.
  this->argc_ = argc;
  this->argv_ = new char *[argc];
  for (int i = 0; i < argc; i++)
    this->argv_[i] = argv[i];


  this->client_number_ = client_number;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      char buf[64];
      ACE_OS::sprintf (buf, "thread_%lx", ACE_reinterpret_cast (long, this));

      this->orb_var_ =
        CORBA::ORB_init (this->argc_,
                         this->argv_,
                         buf
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->object_key_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "The IOR is nil, not able to get the object.\n"),
                            -1);


      CORBA::Object_var object_var =
        this->orb_var_->string_to_object (this->object_key_
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object_var.in()))
          ACE_ERROR_RETURN ((LM_ERROR,
                             "No proper object has been returned.\n"),
                            -1);

      this->mT_Object_var_ = MT_Object::_narrow (object_var.in()
                                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->mT_Object_var_.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "We have no proper reference to the Object.\n"),
                            -1);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "We have a proper reference to the Object.\n"));

      CORBA::Object_var poa_object =
        this->orb_var_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "MT_Client::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}


// This function runs the test.

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ORB_Manager orb_manager;

      int r = orb_manager.init (argc,
                                argv
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (r != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: ORB_Manager initialization failed.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG,"\n\tMT_Client: client\n\n"));

      int i;
      int threads = 1;

      for (i = 0; i < argc; i++)
        if (ACE_OS::strcmp (argv[i], "-n") == 0)
          threads = ACE_OS::atoi(argv[i + 1]);

      // create a separate server thread
      ACE_Thread_Manager server_thr_mgr;
      // starting the server thread
      MT_Server_Task *server = new MT_Server_Task (&server_thr_mgr,
                                                   argc,
                                                   argv,
                                                   &orb_manager);
      if (server->activate () != 0)
        {
          delete server;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "CLIENT ERROR: Unable to activate "
                             "MT_Server_Task.\n"),
                            -1);
        }

      // starting the client threads
      MT_Client_Task **clients = new MT_Client_Task*[threads];

      for (i = 0; i < threads; i++)
        clients[i] = new MT_Client_Task (argc, argv, i);

      for (i = 0; i < threads; i++)
        if (clients[i]->activate () != 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                      "CLIENT ERROR: Unable to activate "
                      "MT_Client_Task.\n"),
                     -1);  // @@ Memory leak!

      int result = ACE_Thread_Manager::instance ()->wait ();

      for (i = 0; i < threads; i++)
        delete clients[i];

      delete [] clients;

      // wait for the server thread to end
      result |= server_thr_mgr.wait ();

      delete server;

      return result;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "main");
    }
  ACE_ENDTRY;

  return 1;
}
