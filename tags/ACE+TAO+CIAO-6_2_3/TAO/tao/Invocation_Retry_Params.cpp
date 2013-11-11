// -*- C++ -*-
//
// $Id$

#include "tao/Invocation_Retry_Params.h"
#include "tao/Invocation_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::Invocation_Retry_Params::Invocation_Retry_Params (void)
  : forward_on_reply_closed_limit_ (0)
  , init_retry_delay_ (0, 100000) // Set default to 0.1 seconds
{
      this->forward_on_exception_limit_[FOE_OBJECT_NOT_EXIST] = 0;
      this->forward_on_exception_limit_[FOE_COMM_FAILURE] = 0;
      this->forward_on_exception_limit_[FOE_TRANSIENT] = 0;
      this->forward_on_exception_limit_[FOE_INV_OBJREF] = 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
