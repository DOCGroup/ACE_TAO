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

      virtual void ping ();
      virtual void farewell ();
  private:
      CORBA::ORB_ptr orb_;
};

#endif /* SHAREDINTF_I_H_  */
