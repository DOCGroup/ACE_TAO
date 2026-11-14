
//=============================================================================
/**
 *  @file    local_server.cpp
 *
 *  This server will run the ORB briefly and then make
 *  several calls on the distant MT Object.
 *
 *  @author Michael Kircher
 */
//=============================================================================

#include "local_server.h"
#include "IOR_File.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"
#include "tao/Utils/ORB_Manager.h"

MT_Server_Task::MT_Server_Task (ACE_Thread_Manager *thr_mgr_ptr,
                                int argc,
                                ACE_TCHAR **argv,
                                TAO_ORB_Manager *orb_manager_ptr)
  : ACE_Task<ACE_SYNCH> (thr_mgr_ptr),
    argc_ (argc),
    argv_ (argv),
    orb_manager_ptr_ (orb_manager_ptr)
{
}

int
MT_Server_Task::svc ()
{
  if (this->mT_Server_.init (this->argc_, this->argv_, this->orb_manager_ptr_) == -1)
    return -1;
  return this->mT_Server_.run_ORB_briefly ();
}

int
MT_Server::read_ior (ACE_TCHAR const *filename)
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
MT_Server::parse_args ()
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_.data (), ACE_TEXT("d:f:g:h:i:n:s:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        ++TAO_debug_level;
        break;
      case 'f': // The local server thread uses the first server's IOR.
        if (this->read_ior (get_opts.opt_arg ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s\n",
                             get_opts.opt_arg ()),
                            -1);
        break;
      case 'g':
      case 'h':
      case 'i':
      case 'n':
        break;
      case 's':
        this->iterations_ = ACE_OS::atoi (get_opts.opt_arg ());
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
                           this->argv_[0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
MT_Server::init (int argc,
                 ACE_TCHAR **argv,
                 TAO_ORB_Manager *orb_manager_ptr)
{
  this->argc_ = argc;
  this->argv_.assign (argv, argv + argc + 1);
  this->orb_manager_ptr_ = orb_manager_ptr;
  if (this->orb_manager_ptr_ == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "MT_Server::init: ORB_Manager is nil!\n"),
                       -1);

  try
    {
      // Call the init of TAO_ORB_Manager to create a child POA
      // under the root POA.
      if (this->orb_manager_ptr_->init_child_poa (this->argc_,
                                                  this->argv_.data (),
                                                  "child_poa") == -1)
        return -1;

      if (this->parse_args () == -1)
        return -1;

      this->str_  =
        this->orb_manager_ptr_->activate_under_child_poa ("MT",
                                                          &this->mT_Object_i_);

      // retrieve the object reference to the distant mt object
      if (this->object_key_.empty ())
        ACE_ERROR_RETURN ((LM_ERROR,
                           "The IOR is nil, not able to get the object.\n"),
                          -1);

      CORBA::ORB_var orb_var = this->orb_manager_ptr_->orb ();

      CORBA::Object_var object_var =
        orb_var->string_to_object (this->object_key_.c_str ());

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
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Server::init");
      return -1;
    }

  return 0;
}

MT_Server::~MT_Server ()
{
  try
    {
      if (this->orb_manager_ptr_)
        this->orb_manager_ptr_->deactivate_under_child_poa (this->str_.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("MT_Server::~MT_Server");
    }
}

int
MT_Server::run_ORB_briefly ()
{
  if (this->orb_manager_ptr_->activate_poa_manager () == -1)
    return -1;

  if (this->iterations_ > 0)
    {
      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) MT_Server::run: "
                      "going to call distant MT Object\n"));

          PortableServer::POA_var root_poa =
            this->orb_manager_ptr_->root_poa ();

          // The servant registered in the child POA is already active, so use
          // a separate servant for the nested upcall on the root POA.
          MT_Object_i nested_servant;
          PortableServer::ObjectId_var id =
            root_poa->activate_object (&nested_servant);

          try
            {
              CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());
              MT_Object_var tmp =
                MT_Object::_narrow (object_act.in ());

              for (unsigned int i = 0; i < this->iterations_; ++i)
                this->mT_Object_var_->yadda (0, tmp.in ());

              root_poa->deactivate_object (id.in ());
            }
          catch (CORBA::Exception const &)
            {
              root_poa->deactivate_object (id.in ());
              throw;
            }
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("MT_Server::run_ORB_briefly");
          return -1;
        }
    }
  return 0;
}
