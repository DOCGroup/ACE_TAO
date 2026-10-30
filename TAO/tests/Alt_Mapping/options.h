
//=============================================================================
/**
 *  @file    options.h
 *
 *  Options for the Param_Test application
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
  enum class Test_Type
  {
    None,
    Unbounded_String,
    Unbounded_Struct_Sequence
  };

  enum class Invoke_Type
  {
    SII,
    DII
  };

  /// constructor
  Options ();
  Options (Options const &) = delete;
  Options (Options &&) = delete;
  Options &operator= (Options const &) = delete;
  Options &operator= (Options &&) = delete;

  /// destructor
  ~Options () = default;

  /// Parses the arguments passed on the command line.
  int parse_args (int argc, ACE_TCHAR **argv);

  /// return the IOR for the servant
  char const * param_test_ior () const;

  /// what test to run
  Test_Type test_type () const;

  /// whether to use SII or DII
  Invoke_Type invoke_type () const;

  /// number of times to run the test
  CORBA::ULong loop_count () const;

  /// whether debug option is on or not
  CORBA::Boolean debug () const;

  /// If we should request the server to shutdown.
  CORBA::Boolean shutdown () const;

private:
  /// Function to read the servant IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// IOR for the servant
  CORBA::String_var ior_;

  /// what test to run
  Test_Type test_type_;

  /// whether SII or DII
  Invoke_Type invoke_type_;

  /// Number of times to do the "test_*" operations.
  CORBA::ULong loop_count_ {};

  /// debugging output values
  bool debug_ {};

  /// server shutdown flag.
  bool shutdown_ {};
};

using OPTIONS = ACE_Singleton<Options, TAO_SYNCH_RECURSIVE_MUTEX>;

#endif /* OPTIONS_H */
