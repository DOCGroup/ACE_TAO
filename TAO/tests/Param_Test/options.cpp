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
//    Aniruddha Gokhale
//
// ============================================================================

#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "options.h"

ACE_RCSID(Param_Test, options, "$Id$")

// Constructor.p
Options::Options (void)
  : ior_ (0),
    test_type_ (Options::NO_TEST),
    invoke_type_ (Options::SII),
    loop_count_ (1),
    debug_ (0),
    shutdown_ (0)
{
}

Options::~Options (void)
{
  // Free resources
  CORBA::string_free (this->ior_);
  this->ior_ = 0;
}

// Parses the command line arguments and returns an error status.

int
Options::parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "xdn:f:i:t:k:");
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
        this->loop_count_ = (CORBA::ULong) ACE_OS::atoi (get_opts.optarg);
        break;

      case 'f':
        result = this->read_ior (get_opts.optarg);

        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);

        break;

     case 'k':
        CORBA::string_free (this->ior_);
        this->ior_ = CORBA::string_dup (get_opts.optarg);
        break;

      case 'i':  // invocation
        if (!ACE_OS::strcmp (get_opts.optarg, "dii"))
          this->invoke_type_ = Options::DII;
        break;

      case 't': // data type
        if (!ACE_OS::strcmp (get_opts.optarg, "short"))
          this->test_type_ = Options::TEST_SHORT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ulonglong"))
          this->test_type_ = Options::TEST_ULONGLONG;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ubstring"))
          this->test_type_ = Options::TEST_UB_STRING;
        else if (!ACE_OS::strcmp (get_opts.optarg, "bdstring"))
          this->test_type_ = Options::TEST_BD_STRING;
        else if (!ACE_OS::strcmp (get_opts.optarg, "fixed_struct"))
          this->test_type_ = Options::TEST_FIXED_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ub_strseq"))
          this->test_type_ = Options::TEST_UB_STRING_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "bd_strseq"))
          this->test_type_ = Options::TEST_BD_STRING_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "var_struct"))
          this->test_type_ = Options::TEST_VAR_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "nested_struct"))
          this->test_type_ = Options::TEST_NESTED_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "objref_struct"))
          this->test_type_ = Options::TEST_OBJREF_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ub_struct_seq"))
          this->test_type_ = Options::TEST_UB_STRUCT_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "bd_struct_seq"))
          this->test_type_ = Options::TEST_BD_STRUCT_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "objref"))
          this->test_type_ = Options::TEST_OBJREF;
        else if (!ACE_OS::strcmp (get_opts.optarg, "typecode"))
          this->test_type_ = Options::TEST_TYPECODE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "any"))
          this->test_type_ = Options::TEST_ANY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "objref_sequence"))
          this->test_type_ = Options::TEST_OBJREF_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "any_sequence"))
          this->test_type_ = Options::TEST_ANYSEQ;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ub_short_sequence"))
          this->test_type_ = Options::TEST_UB_SHORTSEQ;
        else if (!ACE_OS::strcmp (get_opts.optarg, "bd_short_sequence"))
          this->test_type_ = Options::TEST_BD_SHORTSEQ;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ub_long_sequence"))
          this->test_type_ = Options::TEST_UB_LONGSEQ;
        else if (!ACE_OS::strcmp (get_opts.optarg, "bd_long_sequence"))
          this->test_type_ = Options::TEST_BD_LONGSEQ;
        else if (!ACE_OS::strcmp (get_opts.optarg, "fixed_array"))
          this->test_type_ = Options::TEST_FIXED_ARRAY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "var_array"))
          this->test_type_ = Options::TEST_VAR_ARRAY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "multdim_array"))
          this->test_type_ = Options::TEST_MULTDIM_ARRAY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "exception"))
          this->test_type_ = Options::TEST_EXCEPTION;
        else if (!ACE_OS::strcmp (get_opts.optarg, "big_union"))
          this->test_type_ = Options::TEST_BIG_UNION;
        else if (!ACE_OS::strcmp (get_opts.optarg, "complex_any"))
          this->test_type_ = Options::TEST_COMPLEX_ANY;
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
Options::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (f_handle);
  this->ior_ = ior_buffer.read ();

  if (this->ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
  return 0;
}

char *
Options::param_test_ior (void)
{
  return this->ior_;
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
