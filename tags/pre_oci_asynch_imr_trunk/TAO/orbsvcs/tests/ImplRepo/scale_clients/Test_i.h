/* -*- C++ -*- $Id$ */

#ifndef TEST_I_H_
#define TEST_I_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class  Test_i : public virtual POA_Test
{
public:

  Test_i (CORBA::Short num_requests_expected);

  virtual ~Test_i ();

  virtual CORBA::Short get_num_requests (CORBA::Short delay_secs);

  /// Valid after shutdown () is called.
  static bool expected_requests_made ();

private:

  CORBA::Short num_requests_expected_;
  CORBA::Short num_requests_made_;

  static bool expected_requests_made_;
};

#endif /* TEST_I_H_ */
