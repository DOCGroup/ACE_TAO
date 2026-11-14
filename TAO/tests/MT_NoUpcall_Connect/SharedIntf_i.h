#ifndef SHAREDINTF_I_H_
#define SHAREDINTF_I_H_

#include <string>
#include "SharedIntfS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Test_Idl_SharedIntf_i : public virtual POA_Test_Idl::SharedIntf
{
public:
  explicit Test_Idl_SharedIntf_i (CORBA::ORB_ptr orb);
  ~Test_Idl_SharedIntf_i () override = default;
  void set_upper (std::string const& ior);

  void do_upcall () override;
  void ping () override;
  void farewell () override;

  Test_Idl_SharedIntf_i (Test_Idl_SharedIntf_i const&) = delete;
  Test_Idl_SharedIntf_i (Test_Idl_SharedIntf_i&&) = delete;
  Test_Idl_SharedIntf_i& operator= (Test_Idl_SharedIntf_i const&) = delete;
  Test_Idl_SharedIntf_i& operator= (Test_Idl_SharedIntf_i&&) = delete;

private:
  std::string upper_ior_;
  CORBA::ORB_ptr const orb_; // Borrowed; the ORB outlives its servant.
};

#endif /* SHAREDINTF_I_H_  */
