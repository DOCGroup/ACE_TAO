// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    driver.cpp
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ============================================================================

#include "DynAny_i.h"
#include "DynStruct_i.h"
#include "DynSequence_i.h"
#include "DynArray_i.h"
#include "DynEnum_i.h"
#include "DynUnion_i.h"
#include "da_testsC.cpp"

int main (int argc, char* argv[])
{
  CORBA::Environment env;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, env);

  DynAnyTests::test_union* tu;

  tu->_d (DynAnyTests::SECOND);

//  DynAnyTests::enclosed_struct es_in;
//  tu.ts ().l = -222;
//  const char* val = "boing";
//  tu->str (val);

  tu->tc (CORBA::_tc_double);

  CORBA_Any a;
  a <<= tu;

//  DynUnion_i du (DynAnyTests::_tc_test_union);
  DynUnion_i du (a);

//  du.from_any (a, env);

  CORBA_Any* a_out = du.to_any (env);

  DynAnyTests::test_union* tu_out;
//  DynAnyTests::test_enum te_out;
  *a_out >>= tu_out;

  du.destroy (env);

  return 0;
}