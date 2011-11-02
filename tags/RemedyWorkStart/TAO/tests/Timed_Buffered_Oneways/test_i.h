// -*- C++ -*-

//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H
#define TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H

#include "testS.h"

/**
 * @class test_i
 *
 * @brief Simple implementation.
 */
class test_i : public POA_test
{
public:
  /// ctor.
  test_i (CORBA::ORB_ptr orb);

  // = The test interface methods.
  void method (CORBA::ULong request_number,
               CORBA::Long start_time,
               const test::data &,
               CORBA::ULong work);

  void flush (void);

  void shutdown (CORBA::Long start_time);

private:
  /// The ORB.
  CORBA::ORB_var orb_;
};

#endif /* TAO_TIMED_BUFFERED_ONEWAYS_TEST_I_H */
