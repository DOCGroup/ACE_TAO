// $Id

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

#if !defined (HELPER_H)
#define HELPER_H

#include "param_testC.h"

#define TEST_BUFSIZE 128

class Generator
{
public:
  Generator (void);
  // constructor

  ~Generator (void);
  // destructor

  CORBA::Short gen_short (void);
#if 0
  CORBA::Long gen_long (void);
  CORBA::Char gen_char (void);
  CORBA::Octet gen_octet (void);
  CORBA::Float gen_float (void);
  CORBA::Double gen_double (void);
#endif
  CORBA::String gen_string (void);
private:
};

typedef ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX> GENERATOR;
#endif /* !defined */
