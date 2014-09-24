/* -*- C++ -*- $Id$ */

#ifndef TEST_I_H_
#define TEST_I_H_

#include "TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Terminator;

class  Test_i : public virtual POA_Test
{
public:
  //Constructor
  Test_i (short server_num,
          Terminator &terminator,
          PortableServer::POA_ptr pa,
          PortableServer::POA_ptr pb);

  //Destructor
  virtual ~Test_i ();

  virtual CORBA::Short get_server_num (void);

private:
  void reset (PortableServer::POAManager_var &mgr, bool pause);

  PortableServer::POAManager_var mgr_a;
  PortableServer::POAManager_var mgr_b;

  CORBA::Short server_num_;
  Terminator& terminator_;
};

#endif /* TEST_I_H_ */
