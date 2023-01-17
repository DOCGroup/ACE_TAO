
//=============================================================================
/**
 *  @file    test_wrapper.h
 *
 *  Header file for the DynAny basic test manager
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_WRAPPER_H)
#define TEST_WRAPPER_H

template <class T>
class Test_Wrapper
{
public:
  typedef T TEST_OBJECT;

  // constructor and destructor
  Test_Wrapper (T *);

  ~Test_Wrapper ();

  /**
   * @brief Run the test.
   *
   * @return The number of errors detected.
   */
  int run_test ();

private:
  // This does all the work.
  TEST_OBJECT* test_object_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "test_wrapper.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TEST_WRAPPER_H */
