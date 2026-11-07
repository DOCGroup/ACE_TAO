
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

#include <memory>
#include <utility>

template <class T>
class Test_Wrapper
{
public:
  explicit Test_Wrapper (std::unique_ptr<T> test_object)
    : test_object_ (std::move (test_object))
  {
  }
  ~Test_Wrapper () = default;
  Test_Wrapper (Test_Wrapper const&) = delete;
  Test_Wrapper (Test_Wrapper&&) = delete;
  Test_Wrapper& operator= (Test_Wrapper const&) = delete;
  Test_Wrapper& operator= (Test_Wrapper&&) = delete;

  /**
   * @brief Run the test.
   *
   * @return The number of errors detected.
   */
  int run_test ();

private:
  // This does all the work.
  std::unique_ptr<T> test_object_;
};

#include "test_wrapper.cpp"

#endif /* TEST_WRAPPER_H */
