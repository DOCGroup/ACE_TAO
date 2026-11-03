#ifndef SHAREDINTF_I_H_
#define SHAREDINTF_I_H_

#include "SharedIntfS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Test_Idl_SharedIntf_i : public virtual POA_Test_Idl::SharedIntf
{
public:
  Test_Idl_SharedIntf_i (CORBA::ORB_ptr orb);
  ~Test_Idl_SharedIntf_i () override = default;

  Test_Idl_SharedIntf_i (Test_Idl_SharedIntf_i const&) = delete;
  Test_Idl_SharedIntf_i (Test_Idl_SharedIntf_i&&) = delete;
  Test_Idl_SharedIntf_i& operator= (Test_Idl_SharedIntf_i const&) = delete;
  Test_Idl_SharedIntf_i& operator= (Test_Idl_SharedIntf_i&&) = delete;

  void ping () override;
  void farewell () override;
private:
  CORBA::ORB_ptr orb_;
};

#endif /* SHAREDINTF_I_H_  */
