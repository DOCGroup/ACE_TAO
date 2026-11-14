
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  Start one server thread calling a distant MT Object several times,
 *  also starting several client threads which call the MT Object too.
 *  The server does nested upcalls.
 *
 *  @author Michael Kircher
 */
//=============================================================================

#include "client.h"
#include "local_server.h"
#include "IOR_File.h"
#include "tao/debug.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Truncate.h"

#include <memory>

MT_Client_Task::MT_Client_Task (int argc, ACE_TCHAR **argv,
                                int client_number)
  : argc_ (argc),
    argv_ (argv),
    client_number_ (client_number)
{
}

int
MT_Client_Task::svc ()
{
  if (this->mT_Client_.init (this->argc_, this->argv_, this->client_number_) == -1)
    return -1;
  return this->mT_Client_.run ();
}

int
MT_Client::read_ior (ACE_TCHAR const *filename)
{
  // Open the file for reading.
  ACE_HANDLE const f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle, true);
  MT_ACE_Allocated_Buffer ior_buffer_data {ior_buffer.read ()};
  if (!ior_buffer_data)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior.\n"),
                       -1);

  this->object_key_.assign (ior_buffer_data.get ());
  return 0;
}

int
MT_Client::parse_args ()
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_.data (), ACE_TEXT("df:g:h:i:n:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        ++TAO_debug_level;
        break;
      case 'f': // read the IOR from the file.
        if ((this->client_number_ % 2) == 0)
          {
            if (this->read_ior (get_opts.opt_arg ()) < 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Unable to read ior from %s\n",
                                 get_opts.opt_arg ()),
                                -1);
          }
        break;
      case 'g': // read the IOR from the file.
        if ((this->client_number_ % 2) == 1)
          {
            if (this->read_ior (get_opts.opt_arg ()) < 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Unable to read ior from %s\n",
                                 get_opts.opt_arg ()),
                                -1);
          }
        break;
      case 'i':
        this->iterations_ = ACE_OS::atoi (get_opts.opt_arg ());
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
                           this->argv_[0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
MT_Client::run ()
{
  try
    {
      for (unsigned long i = 0; i < this->iterations_; i++)
        {
          // call the recursive object MT_Object for nested upcalls
          // testing
          this->mT_Object_var_->yadda (0, 0);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client:run");
      return -1;
    }

  return 0;
}

int
MT_Client::init (int argc, ACE_TCHAR **argv,
                 int client_number)
{
  // Make a copy of argv since ORB_init will change it.
  this->argc_ = argc;
  this->argv_.assign (argv, argv + argc + 1);

  this->client_number_ = client_number;

  try
    {
      char buf[64] {};
      ACE_OS::sprintf (buf, "thread_%lx", ACE_Utils::truncate_cast<long> ((intptr_t)this));

      this->orb_var_ =
        CORBA::ORB_init (this->argc_,
                         this->argv_.data (),
                         buf);

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->object_key_.empty ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "The IOR is nil, not able to get the object.\n"),
                          -1);

      CORBA::Object_var object_var =
        this->orb_var_->string_to_object (this->object_key_.c_str ());

      if (CORBA::is_nil (object_var.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "No proper object has been returned.\n"),
                          -1);

      this->mT_Object_var_ = MT_Object::_narrow (object_var.in ());

      if (CORBA::is_nil (this->mT_Object_var_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "We have no proper reference to the Object.\n"),
                            -1);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "We have a proper reference to the Object.\n"));

      CORBA::Object_var poa_object =
        this->orb_var_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Client::init");
      return -1;
    }

  return 0;
}

// This function runs the test.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      TAO_ORB_Manager orb_manager;

      int const result = orb_manager.init (argc, argv);

      if (result != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: ORB_Manager initialization failed.\n"),
                          -1);

      ACE_DEBUG ((LM_DEBUG, "\n\tMT_Client: client\n\n"));

      int threads = 1;

      for (int i = 0; i + 1 < argc; ++i)
        if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-n")) == 0)
          threads = ACE_OS::atoi (argv[i + 1]);

      if (threads <= 0)
        ACE_ERROR_RETURN ((LM_ERROR, "The number of client threads must be positive.\n"), -1);

      // create a separate server thread
      ACE_Thread_Manager server_thr_mgr;
      // starting the server thread
      std::unique_ptr<MT_Server_Task> server =
        std::make_unique<MT_Server_Task> (&server_thr_mgr, argc, argv, &orb_manager);
      if (server->activate () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "CLIENT ERROR: Unable to activate MT_Server_Task.\n"),
                          -1);

      // starting the client threads
      std::vector<std::unique_ptr<MT_Client_Task>> clients;
      clients.reserve (threads);

      for (int i = 0; i < threads; ++i)
        {
          clients.emplace_back (std::make_unique<MT_Client_Task> (argc, argv, i));
          if (clients.back ()->activate () != 0)
            {
              ACE_Thread_Manager::instance ()->wait ();
              server_thr_mgr.wait ();
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "CLIENT ERROR: Unable to activate MT_Client_Task.\n"),
                                -1);
            }
        }

      int const client_result = ACE_Thread_Manager::instance ()->wait ();
      // wait for the server thread to end
      int const server_result = server_thr_mgr.wait ();

      server.reset ();
      orb_manager.fini ();
      return client_result | server_result;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("main");
    }

  return -1;
}
