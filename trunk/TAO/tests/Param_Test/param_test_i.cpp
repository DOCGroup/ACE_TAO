// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "tao/corba.h"
#include "param_test_i.h"

// Constructor

Param_Test_i::Param_Test_i (const char *obj_name)
  : POA_Param_Test (obj_name)
{
}

// Destructor

Param_Test_i::~Param_Test_i (void)
{
}

// Cube an octet

CORBA::Short
Param_Test_i::test_short (CORBA::Short s1,
                          CORBA::Short &s2,
                          CORBA::Short_out s3,
                          CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  s2 = s1 * 2;
  s3 = s1 * 3;
  return s1 * 4;
}
