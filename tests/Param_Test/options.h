
//=============================================================================
/**
 *  @file    options.h
 *
 *  $Id$
 *
 *  Options for the Param_Test application
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef OPTIONS_H
#define OPTIONS_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/CORBA_String.h"
#include "tao/orbconf.h"

class Options
{
public:
  enum TEST_TYPE
  {
    NO_TEST,
    TEST_SHORT,
    TEST_ULONGLONG,
    TEST_UB_STRING,
    TEST_UB_WSTRING,
    TEST_BD_STRING,
    TEST_BD_WSTRING,
    TEST_FIXED_STRUCT,
    TEST_UB_STRING_SEQUENCE,
    TEST_BD_STRING_SEQUENCE,
    TEST_UB_WSTRING_SEQUENCE,
    TEST_BD_WSTRING_SEQUENCE,
    TEST_VAR_STRUCT,
    TEST_NESTED_STRUCT,
    TEST_RECURSIVE_STRUCT,
    TEST_OBJREF_STRUCT,
    TEST_UB_STRUCT_SEQUENCE,
    TEST_BD_STRUCT_SEQUENCE,
    TEST_UB_ARRAY_SEQUENCE,
    TEST_BD_ARRAY_SEQUENCE,
    TEST_OBJREF,
    TEST_TYPECODE,
    TEST_ANY,
    TEST_OBJREF_SEQUENCE,
    TEST_ANYSEQ,
    TEST_UB_SHORTSEQ,
    TEST_BD_SHORTSEQ,
    TEST_UB_LONGSEQ,
    TEST_BD_LONGSEQ,
    TEST_FIXED_ARRAY,
    TEST_VAR_ARRAY,
    TEST_MULTDIM_ARRAY,
    TEST_EXCEPTION,
    TEST_BIG_UNION,
    TEST_SMALL_UNION,
    TEST_RECURSIVE_UNION,
    TEST_COMPLEX_ANY
  };

  enum INVOKE_TYPE
  {
    SII,
    DII
  };

  /// constructor
  Options (void);

  /// destructor
  ~Options (void);

  /// Parses the arguments passed on the command line.
  int parse_args (int argc, ACE_TCHAR **argv);

  /// return the IOR for the servant
  char const * param_test_ior (void) const;

  /// what test to run
  TEST_TYPE test_type (void);

  /// whether to use SII or DII
  INVOKE_TYPE invoke_type (void);

  /// number of times to run the test
  CORBA::ULong loop_count (void);

  /// whether debug option is on or not
  CORBA::Boolean debug (void) const;

  /// If we should request the server to shutdown.
  CORBA::Boolean shutdown (void) const;

private:
  /// Function to read the servant IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// IOR for the servant
  CORBA::String_var ior_;

  /// what test to run
  TEST_TYPE test_type_;

  /// whether SII or DII
  INVOKE_TYPE invoke_type_;

  /// Number of times to do the "test_*" operations.
  CORBA::ULong loop_count_;

  /// debugging output values
  CORBA::Boolean debug_;

  /// server shutdown flag.
  CORBA::Boolean shutdown_;
};

typedef ACE_Singleton<Options, TAO_SYNCH_RECURSIVE_MUTEX> OPTIONS;

#endif /* OPTIONS_H */
