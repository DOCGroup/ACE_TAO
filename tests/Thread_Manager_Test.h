// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Manager_Test.h
//
// = DESCRIPTION
//      This file contains class definitions needed for template
//      instnatiation in the Thread_Manager_Test.cpp file.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
//
// ============================================================================

#ifndef __THREAD_MANAGER_TEST_H
#define __THREAD_MANAGER_TEST_H


class Signal_Catcher
  // = TITLE
  //     Keeps track of whether a thread has been signaled.
{
public:
  Signal_Catcher (void): signaled_ (0) {}

  sig_atomic_t signaled (void)
  {
    return this->signaled_;
  }

  void signaled (sig_atomic_t s)
  {
    this->signaled_ = s;
  }

private:
  sig_atomic_t signaled_;
};

#endif /* __THREAD_MANAGER_TEST_H */
