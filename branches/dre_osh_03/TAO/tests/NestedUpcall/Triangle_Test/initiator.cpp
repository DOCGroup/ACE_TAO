// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    initiator.cpp
//
// = DESCRIPTION
//    This class implements a simple server for the
//    Nested Upcalls - Triangle test.
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#include "initiator.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Triangle_Test, initiator, "$Id$")

Initiator_Server::Initiator_Server (void)
  : object_A_key_ (0),
    object_B_key_ (0),
    object_A_var_ (0),
    object_B_var_ (0),
    initiator_i_ptr_ (0)
{
}

// Reads the Object A/B IOR from a file
// A_B == 0, means read Object A's IOR
// A_B == 1, means read Object B's IOR

int
Initiator_Server::read_ior (char *filename, unsigned int A_B)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename,0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle);

  if (A_B == 0)
  {
    this->object_A_key_ = ior_buffer.read ();
    if (this->object_A_key_ == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to allocate memory to read ior: %p\n"),
                        -1);
  }
  else
  {
    this->object_B_key_ = ior_buffer.read ();
    if (this->object_B_key_ == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to allocate memory to read ior: %p\n"),
                        -1);
  }

  ACE_OS::close (f_handle);
  return 0;
}


int
Initiator_Server::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "df:g:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg (),0);
        // read IOR for Object A
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
            break;
      case 'g': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg (),1);
        // read IOR for Object A
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
            break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f] <object_A_ior_file>"
                           " [-g] <object_B_ior_file>"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Initiator_Server::init (int argc,
                       char** argv
                       ACE_ENV_ARG_DECL)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  this->parse_args ();
  // ~~ check for the return value here

  if (this->object_A_key_ == 0 || this->object_B_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "%s: The two objects A and B are missing\n",
                      this->argv_[0]),
                      -1);

  ACE_TRY
    {
      // Get Object A

      CORBA::Object_var object_A_obj_var =
        this->orb_manager_.orb()->string_to_object (this->object_A_key_
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_A_var_ =
        Object_A::_narrow (object_A_obj_var.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->object_A_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid object A key <%s>\n",
                           object_A_key_),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Object A IOR: %s\n",
                  this->object_A_key_));
      ACE_DEBUG ((LM_DEBUG, "Object A received OK\n"));

      // Get Object B

      CORBA::Object_var object_B_obj_var =
        this->orb_manager_.orb()->string_to_object (this->object_B_key_
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_B_var_ =
        Object_B::_narrow (object_B_obj_var.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->object_B_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid object b key <%s>\n",
                           object_B_key_),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  "Object A IOR: %s\n",
                  this->object_A_key_));
      ACE_DEBUG ((LM_DEBUG, "Object B received OK\n"));

      this->orb_manager_.activate_poa_manager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;

  ACE_NEW_RETURN (this->initiator_i_ptr_,
                  Initiator_i(this->object_A_var_.in(),
                              this->object_B_var_.in()),
                  -1);

  this->str_ =
    this->orb_manager_.activate (this->initiator_i_ptr_
                                 ACE_ENV_ARG_PARAMETER);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              this->str_.in ()));


  return 0;
}


int
Initiator_Server::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Initiator_Server::run: Trying to invoke "
                  "foo on Object A\n"));

      Initiator_var initiator =
        this->initiator_i_ptr_->_this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_A_var_->foo (initiator.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  "Initiator_Server::run: Returned from invoke "
                  "foo on Object A\n"));
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Initiator_Server::~Initiator_Server (void)
{
  if (this->object_A_key_ != 0)
    ACE_OS::free (this->object_A_key_);
  if (this->object_B_key_ != 0)
    ACE_OS::free (this->object_B_key_);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_manager_.deactivate (this->str_.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Initiator_Server::~Initiator_Server");
    }
  ACE_ENDTRY;

  delete this->initiator_i_ptr_;
}

int
main (int argc, char *argv[])
{
  Initiator_Server initiator_Server;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls.Triangle_Test: Initiator Server \n \n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      int retval =
        initiator_Server.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (retval == -1)
        return 1;
      else
        {
          initiator_Server.run (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
