// $Id:

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

#include "options.h"

// Constructor.p
Options::Options (void)
  : param_test_key_ (CORBA::string_dup ("param_test")),
    hostname_ (CORBA::string_dup (ACE_DEFAULT_SERVER_HOST)),
    portnum_ (TAO_DEFAULT_SERVER_PORT),
    test_type_ (Options::NO_TEST),
    invoke_type_ (Options::SII),
    loop_count_ (1),
    debug_ (0)
{
}

Options::~Options (void)
{
  // Free resources
  CORBA::string_free (this->param_test_key_);
  this->param_test_key_ = 0;
  CORBA::string_free (this->hostname_);
  this->hostname_ = 0;
}

// Parses the command line arguments and returns an error status.
int
Options::parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "dn:h:p:k:i:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        this->debug_ = 1;
        break;
      case 'n':			// loop count
        this->loop_count_ = (CORBA::ULong) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'h':
        CORBA::string_free (this->hostname_); // release old one
        this->hostname_ = CORBA::string_dup (get_opts.optarg);
        break;
      case 'p':
        this->portnum_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'k':			// stringified objref
        CORBA::string_free (this->param_test_key_); // release old one
        this->param_test_key_ = CORBA::string_dup (get_opts.optarg);
        break;
      case 'i':  // invocation
        if (!ACE_OS::strcmp (get_opts.optarg, "dii"))
          this->invoke_type_ = Options::DII;
        break;
      case 't': // data type
        if (!ACE_OS::strcmp (get_opts.optarg, "short"))
          this->test_type_ = Options::TEST_SHORT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "ubstring"))
          this->test_type_ = Options::TEST_UNBOUNDED_STRING;
        else if (!ACE_OS::strcmp (get_opts.optarg, "fixed_struct"))
          this->test_type_ = Options::TEST_FIXED_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "strseq"))
          this->test_type_ = Options::TEST_STRING_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "var_struct"))
          this->test_type_ = Options::TEST_VAR_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "nested_struct"))
          this->test_type_ = Options::TEST_NESTED_STRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "struct_seq"))
          this->test_type_ = Options::TEST_STRUCT_SEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "objref"))
          this->test_type_ = Options::TEST_OBJREF;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-k Param_Test-obj-ref-key]"
                           " [-h hostname]"
                           " [-i invocation (sii/dii)]"
                           " [-t data type]"
                           " [-p port]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

char *
Options::param_test_key (void)
{
  return this->param_test_key_;
}

char *
Options::hostname (void)
{
  return this->hostname_;
}

CORBA::UShort
Options::portnum (void)
{
  return this->portnum_;
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Options, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
