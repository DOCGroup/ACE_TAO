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
  //Constructor
  Test_i (short server_num, CORBA::Short reply_delay_secs);

  //Destructor
  virtual ~Test_i ();

  virtual CORBA::Short get_server_num ();

private:
  CORBA::Short server_num_;
  CORBA::Short reply_delay_secs_;

};

#endif /* TEST_I_H_ */
