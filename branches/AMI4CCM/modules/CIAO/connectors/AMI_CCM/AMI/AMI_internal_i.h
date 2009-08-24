// $Id$

#ifndef AMI_INTERNAL_I_H
#define AMI_INTERNAL_I_H

#include "AMI_internalS.h"
#include "AMI_exec.h"

class AMI_internal_i : public POA_INTERNAL_CCM_AMI::AMI_foo
{
public:
  /// ctor
  AMI_internal_i (
    CORBA::ORB_ptr orb,
    ::CCM_AMI::AMI_foo_ptr foo_receiver);

  // The AMI methods.
  CORBA::Long asynch_foo (const char * in_str,
                 CORBA::String_out out_str);

  void shutdown (void);

private:
  CORBA::ORB_var orb_;
  ::CCM_AMI::AMI_foo_var foo_receiver_;
};

#endif /* AMI_INTERNAL_I_H */

