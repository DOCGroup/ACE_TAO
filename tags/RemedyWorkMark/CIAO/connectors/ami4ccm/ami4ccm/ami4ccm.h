// -*- C++ -*-

// ================================================================
/**
 *  @file ami4ccm.h
 *
 *  $Id$
 *
 *  Include all the required headers to use CCM AMI easily.
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ================================================================

#ifndef CIAO_AMI4CCM_H
#define CIAO_AMI4CCM_H

#include /**/ "ace/pre.h"

#include "connectors/ami4ccm/ami4ccm/ami4ccmC.h"
#include "tao/LocalObject.h"
#include "tao/Messaging/Messaging.h"

namespace CCM_AMI
{
  class AMI4CCM_STUB_Export ExceptionHolder_i
      : public virtual ::CCM_AMI::ExceptionHolder,
        public virtual ::CORBA::LocalObject
  {
  public:
      ExceptionHolder_i (::Messaging::ExceptionHolder* holder);

      /// Rethrow the original exception
      virtual void raise_exception (void);

  private:
      ::Messaging::ExceptionHolder* holder_;
  };
}

#include /**/ "ace/post.h"
#endif /* CIAO_AMI4CCM_H */
