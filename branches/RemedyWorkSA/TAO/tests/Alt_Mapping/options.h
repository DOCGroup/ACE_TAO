
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
    TEST_UB_STRING,
    TEST_UB_STRUCT_SEQUENCE
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
