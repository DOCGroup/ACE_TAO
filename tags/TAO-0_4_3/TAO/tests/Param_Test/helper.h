// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    helper.h
//
// = DESCRIPTION
//    Defines a helper class that can generate values for the parameters used
//    for the Param_Test example
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef HELPER_H
#define HELPER_H

#include "param_testCli.h"

class Generator
{
public:
  Generator (void);
  // constructor

  ~Generator (void);
  // destructor

  CORBA::Short gen_short (void);
  CORBA::Long gen_long (void);
#if 0
  CORBA::Char gen_char (void);
  CORBA::Octet gen_octet (void);
  CORBA::Float gen_float (void);
  CORBA::Double gen_double (void);
#endif
  CORBA::String gen_string (void);
  CORBA::String gen_string (int maxlength);
  const Param_Test::Fixed_Struct gen_fixed_struct (void);
  const Param_Test::Step gen_step (void);
 private:
  Param_Test::Fixed_Struct fixed_struct_;
  Param_Test::Step step_;
};

typedef ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX> GENERATOR;
#endif /* HELPER_H */
