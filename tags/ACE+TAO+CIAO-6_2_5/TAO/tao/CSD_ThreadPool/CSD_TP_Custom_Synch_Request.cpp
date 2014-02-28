// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Custom_Synch_Request.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Custom_Synch_Request.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Custom_Synch_Request::~TP_Custom_Synch_Request()
{
}


void
TAO::CSD::TP_Custom_Synch_Request::dispatch_i()
{
  this->execute_op();
  this->synch_helper_.dispatched();
}


void
TAO::CSD::TP_Custom_Synch_Request::cancel_i()
{
  this->cancel_op();
  this->synch_helper_.cancelled();
}

TAO_END_VERSIONED_NAMESPACE_DECL
