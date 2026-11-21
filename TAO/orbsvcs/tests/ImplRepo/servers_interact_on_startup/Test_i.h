/* -*- C++ -*-  */

#ifndef TEST_I_H_
#define TEST_I_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Test_i : public virtual POA_Test
{
public:
  Test_i (short server_num, CORBA::Short reply_delay_secs);

  ~Test_i () override;

  CORBA::Short get_server_num () override;

private:
  CORBA::Short server_num_;
  CORBA::Short reply_delay_secs_;
};

#endif /* TEST_I_H_ */
