// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    local_server.cpp
//
// = DESCRIPTION
//    This server will run the ORB briefly and then make
//    several calls on the distant MT Object.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "ace/Read_Buffer.h"
#include "local_server.h"

ACE_RCSID(MT_Client_Test, local_server, "$Id$")

MT_Server_Task::MT_Server_Task (ACE_Thread_Manager* thr_mgr_ptr,
                                int argc,
                                char **argv,
                                TAO_ORB_Manager* orb_manager_ptr)
   :ACE_Task<ACE_SYNCH> (thr_mgr_ptr),
    argc_ (argc),
    argv_ (argv),
    orb_manager_ptr_ (orb_manager_ptr)
{
}

int
MT_Server_Task::svc (void)
{
  if (this->mT_Server_.init (this->argc_,
                             this->argv_,
                             this->orb_manager_ptr_) == -1)
    return 1;
  else
    return this->mT_Server_.run_ORB_briefly ();
}


MT_Server::MT_Server ()
  : object_key_ (0),
    ior_output_file_ (0),
    orb_manager_ptr_ (0),
    iterations_ (1)
{
}

// Reads the MT Object IOR from a file
int
MT_Server::read_ior (char *filename)
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



int
MT_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "d:f:g:h:i:n:s:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'h': // read the IOR from the file.
        int result;
        result = this->read_ior (get_opts.optarg);
        // read IOR for MT Object
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;
      case 'f':
      case 'g':
      case 'i':
      case 'n':
        break;
      case 's': this->iterations_ = atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]\n"
                           " [-f] first server ior file\n"
                           " [-g] second server ior file\n"
                           " [-h] third server ior file\n"
                           " [-i] client iterations\n"
                           " [-n] number of client threads\n"
                           " [-s] number of server iterations\n"
                           "\n",
                           argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
MT_Server::init (int argc,
                 char** argv,
                 TAO_ORB_Manager* orb_manager_ptr)
{
  this->argc_ = argc;
  this->argv_ = argv;
  if ((this->orb_manager_ptr_ = orb_manager_ptr) == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "MT_Server::init: ORB_Manager is nil!\n"),
                       -1);

  TAO_TRY
  {

    // Call the init of TAO_ORB_Manager to create a child POA
    // under the root POA.
    this->orb_manager_ptr_->init_child_poa (argc,
                                       argv,
                                       "child_poa",
                                       TAO_TRY_ENV);

    TAO_CHECK_ENV;

    this->parse_args ();
    // ~~ check for the return value here

    CORBA::String_var str  =
      this->orb_manager_ptr_->activate_under_child_poa ("MT",
                                                 &this->mT_Object_i_,
                                                 TAO_TRY_ENV);
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

    // retrieve the object reference to the distant mt object
    if (this->object_key_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "The IOR is nil, not able to get the object.\n"),
                            -1);

    CORBA::ORB_var orb_var = this->orb_manager_ptr_->orb ();

    CORBA::Object_var object_var = orb_var->string_to_object (this->object_key_,
                                                              TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (CORBA::is_nil (object_var.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "No proper object has been returned.\n"),
                          -1);

    this->mT_Object_var_ = MT_Object::_narrow (object_var.in(),
                                               TAO_TRY_ENV);
    TAO_CHECK_ENV;

    if (CORBA::is_nil (this->mT_Object_var_.in()))
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "We have no proper reference to the Object.\n"),
                          -1);
    }

    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "We have a proper reference to the Object.\n"));

  }
  TAO_CATCHANY
  {
    TAO_TRY_ENV.print_exception ("MT_Client::init");
    return -1;
  }
  TAO_ENDTRY;


  return 0;
}

int
MT_Server::run ()
{
  CORBA::Environment env;
  if (this->orb_manager_ptr_->run (env) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "MT_Server::run"),
                      -1);
  return 0;
}

MT_Server::~MT_Server (void)
{
  if (this->object_key_ != 0)
    ACE_OS::free (this->object_key_);
}


int
MT_Server::run_ORB_briefly (void)
{
  if (this->iterations_ > 0)
  {
    CORBA::Environment env;

    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) MT_Server::run: "
                "going to call distant MT Object\n"));

    for (unsigned int i = 0; i < this->iterations_; i++)
    {
      this->mT_Object_var_->yadda (0,
                                   this->mT_Object_i_._this (env),
                                   env);

      if (env.exception () != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "MT_Server::run: failed to call distant MT object\n"),
                          -1);
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) MT_Server::run: "
                  "called distant MT Object i = %d\n",
                  i));
    }
  }
  return 0;
}
