//=============================================================================
/**
 *  @file    initiator.cpp
 *
 *  This class implements a simple server for the
 *  Nested Upcalls - Triangle test.
 *
 *  @author Michael Kircher
 */
//=============================================================================
#include "initiator.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Malloc_Base.h"

#include <memory>
#include <new>

// Read one object's IOR from a file.

int
Initiator_Server::read_ior (ACE_TCHAR const *filename, std::string &ior_key)
{
  // Open the file for reading.
  ACE_HANDLE const f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle, true);
  std::unique_ptr<char, void (*) (char *)> const buffer (
    ior_buffer.read (),
    [] (char *memory)
    {
      if (memory != nullptr)
        ACE_Allocator::instance ()->free (memory);
    });

  if (!buffer)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior\n"),
                      -1);

  ior_key = buffer.get ();
  return 0;
}

int
Initiator_Server::parse_args ()
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("df:g:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'f': // read the IOR from the file.
        if (this->read_ior (get_opts.opt_arg (), this->object_A_key_) < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;
      case 'g': // read the IOR from the file.
        if (this->read_ior (get_opts.opt_arg (), this->object_B_key_) < 0)
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
                        ACE_TCHAR** argv)
{
  // Call the init of TAO_ORB_Manager to create a child POA
  // under the root POA.
  this->orb_manager_.init_child_poa (argc,
                                     argv,
                                     "child_poa");

  this->argc_ = argc;
  this->argv_ = argv;

  if (this->parse_args () != 0)
    return -1;

  if (this->object_A_key_.empty () || this->object_B_key_.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                      "%s: The two objects A and B are missing\n",
                      this->argv_[0]),
                      -1);

  try
    {
      CORBA::ORB_var const orb = this->orb_manager_.orb ();
      // Get Object A

      CORBA::Object_var const object_A_obj_var =
        orb->string_to_object (this->object_A_key_.c_str ());

      this->object_A_var_ = Object_A::_narrow (object_A_obj_var.in ());

      if (CORBA::is_nil (this->object_A_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid object A key <%s>\n",
                           this->object_A_key_.c_str ()),
                          -1);

#if 0
      ACE_DEBUG ((LM_DEBUG,
                  "Object A IOR: %s\n",
                  this->object_A_key_.c_str ()));
#endif /*if 0*/

      ACE_DEBUG ((LM_DEBUG, "Object A received OK\n"));

      // Get Object B

      CORBA::Object_var const object_B_obj_var =
        orb->string_to_object (this->object_B_key_.c_str ());

      this->object_B_var_ = Object_B::_narrow (object_B_obj_var.in ());

      if (CORBA::is_nil (this->object_B_var_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid object b key <%s>\n",
                           this->object_B_key_.c_str ()),
                          -1);

#if 0
      ACE_DEBUG ((LM_DEBUG,
                  "Object B IOR: %s\n",
                  this->object_B_key_.c_str ()));
#endif /*if 0*/

      ACE_DEBUG ((LM_DEBUG, "Object B received OK\n"));

      this->orb_manager_.activate_poa_manager ();
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }

  this->initiator_i_ptr_.reset (
    new (std::nothrow) Initiator_i (this->object_A_var_.in (),
                                    this->object_B_var_.in ()));
  if (!this->initiator_i_ptr_)
    return -1;

  this->str_ = this->orb_manager_.activate (this->initiator_i_ptr_.get ());

#if 0
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              this->str_.in ()));
#endif /*if 0*/

  return 0;
}

int
Initiator_Server::run ()
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Initiator_Server::run: Trying to invoke "
                  "foo on Object A\n"));

      Initiator_var const initiator = this->initiator_i_ptr_->_this ();

      this->object_A_var_->foo (initiator.in ());
      ACE_DEBUG ((LM_DEBUG,
                  "Initiator_Server::run: Returned from invoke "
                  "foo on Object A\n"));
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }

  return 0;
}

Initiator_Server::~Initiator_Server ()
{
  if (!CORBA::is_nil (this->object_A_var_.in ()))
    this->object_A_var_->shutdown ();
  if (!CORBA::is_nil (this->object_B_var_.in ()))
    this->object_B_var_->shutdown ();

  if (this->initiator_i_ptr_ != nullptr)
    {
      try
        {
          this->orb_manager_.deactivate (this->str_.in ());
        }
      catch (CORBA::Exception const &ex)
        {
          ex._tao_print_exception ("Initiator_Server::~Initiator_Server");
        }
    }

}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls.Triangle_Test: Initiator Server \n\n"));

  try
    {
      Initiator_Server initiator_Server;

      int const retval = initiator_Server.init (argc, argv);

      if (retval == -1)
        return 1;
      return initiator_Server.run () == 0 ? 0 : 1;
    }
  catch (CORBA::SystemException const &sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (CORBA::UserException const &userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}
