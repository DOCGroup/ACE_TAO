// $Id$

#ifndef SHAREDINTF_I_H_
#define SHAREDINTF_I_H_

#include "SharedIntfS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Test_Idl_SharedIntf_i : public virtual POA_Test_Idl::SharedIntf
{
public:

  Test_Idl_SharedIntf_i (CORBA::ORB_ptr orb);
  virtual ~Test_Idl_SharedIntf_i (void);
  void set_upper (const char * ior);

  virtual void do_upcall (void);
  virtual void ping (void);
  virtual void farewell (void);
private:
  CORBA::String_var upper_ior;
  CORBA::ORB_ptr orb_;
};

#endif /* SHAREDINTF_I_H_  */
