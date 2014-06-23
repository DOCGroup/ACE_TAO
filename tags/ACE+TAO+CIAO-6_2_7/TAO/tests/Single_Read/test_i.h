
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_SINGLE_READ_TEST_I_H
#define TAO_SINGLE_READ_TEST_I_H

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
  test_i (CORBA::ORB_ptr orb, const ACE_TCHAR *file_name);

  // = The test interface methods.
  void method (CORBA::ULong request_number,
               const test::data &);

  void shutdown (void);

private:
  /// The ORB.
  CORBA::ORB_var orb_;

  CORBA::Boolean client_done_;

  const ACE_TCHAR *client_done_file_;
};

#endif /* TAO_SINGLE_READ_TEST_I_H*/
