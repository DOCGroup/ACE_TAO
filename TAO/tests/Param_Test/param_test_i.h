// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_I_H)
#define	PARAM_TEST_I_H

#include "param_testS.h"

class Param_Test_i : public POA_Param_Test
{
  // = TITLE
  //    Param_Test_i
  // = DESCRIPTION
  //    Implementation of the Param_Test test suite.
public:
  Param_Test_i (const char *obj_name = 0);
  // Constructor

  ~Param_Test_i (void);
  // Destructor

  virtual CORBA::Short test_short (CORBA::Short s1,
                                   CORBA::Short &s2,
                                   CORBA::Short_out s3,
                                   CORBA::Environment &env);
  // test for shorts

  virtual char *test_unbounded_string (const char *s1,
                                       char *&s2,
                                       CORBA::String_out s3,
                                       CORBA::Environment &env);
  // test for unbounded strings
};

#endif /* if !defined */
