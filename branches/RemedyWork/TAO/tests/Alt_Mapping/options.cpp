// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    options.cpp
//
// = DESCRIPTION
//    Options for the Param_Test application
//
// = AUTHORS
//    Jeff Parsonss
//
// ============================================================================

#include "options.h"
#include "tao/debug.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Log_Msg.h"

// Constructor.p
Options::Options (void)
  : ior_ (CORBA::string_dup ("file://test.ior")),
    test_type_ (Options::NO_TEST),
    invoke_type_ (Options::SII),
    loop_count_ (1),
    debug_ (0),
    shutdown_ (0)
{
}

Options::~Options (void)
{
}

// Parses the command line arguments and returns an error status.

int
Options::parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xdn:f:i:t:k:"));
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        this->debug_ = 1;
        break;

      case 'x':
        this->shutdown_ = 1;
        break;

      case 'n':                 // loop count
        this->loop_count_ = (CORBA::ULong) ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'f':
        result = this->read_ior (get_opts.opt_arg ());

        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);

        break;

     case 'k':
        this->ior_ = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;

      case 'i':  // invocation
        if (!ACE_OS::strcmp (get_opts.opt_arg (), ACE_TEXT("dii")))
          this->invoke_type_ = Options::DII;
        break;

      case 't': // data type
        if (!ACE_OS::strcmp (get_opts.opt_arg (), ACE_TEXT("ubstring")))
          this->test_type_ = Options::TEST_UB_STRING;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), ACE_TEXT("ub_struct_seq")))
          this->test_type_ = Options::TEST_UB_STRUCT_SEQUENCE;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f servant-IOR-file]"
                           " [-i invocation (sii/dii)]"
                           " [-t data type]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Get the factory IOR from the file created by the server.
int
Options::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle, true);
  this->ior_ = ior_buffer.read ();

  if (this->ior_.in () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
  return 0;
}

char const *
Options::param_test_ior (void) const
{
  return this->ior_.in ();
}

Options::TEST_TYPE
Options::test_type (void)
{
  return this->test_type_;
}

Options::INVOKE_TYPE
Options::invoke_type (void)
{
  return this->invoke_type_;
}

CORBA::ULong
Options::loop_count (void)
{
  return this->loop_count_;
}

CORBA::Boolean
Options::debug (void) const
{
  return this->debug_;
}

CORBA::Boolean
Options::shutdown (void) const
{
  return this->shutdown_;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Options, ACE_Recursive_Thread_Mutex> *ACE_Singleton<Options, ACE_Recursive_Thread_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
