// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    any_test_i.h
//
// = AUTHOR
//   Michael Kircher
//
// ============================================================================

#ifndef _ANY_TEST_I_HH
#define _ANY_TEST_I_HH

#include "any_testS.h"

// Forward declarations
class Any_Test_i;

// Typedefs.
typedef Any_Test_i *Any_Test_i_ptr;
typedef Any_Test_i_ptr Any_Test_i_ref;

class Any_Test_i : public POA_Any_Test
  // = TITLE
  // = DESCRIPTION
{
public:
  Any_Test_i (const char *obj_name = 0);
  // Constructor

  ~Any_Test_i (void);
  // Destructor

  virtual void try_an_any (const CORBA::Any &a,
                                   CORBA::Environment &env);
};

#endif /* _ANY_TEST_I_HH */
