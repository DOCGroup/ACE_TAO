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

  virtual CORBA::Short get_server_num (void);

  virtual void terminate (void);

private:
};

#endif /* TEST_I_H_ */
