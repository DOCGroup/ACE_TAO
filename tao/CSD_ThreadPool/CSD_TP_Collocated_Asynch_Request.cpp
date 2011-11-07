// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Asynch_Request.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Asynch_Request.inl"
#endif /* ! __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Collocated_Asynch_Request::~TP_Collocated_Asynch_Request()
{
}


void
TAO::CSD::TP_Collocated_Asynch_Request::prepare_for_queue_i()
{
  this->do_clone();
}


void
TAO::CSD::TP_Collocated_Asynch_Request::dispatch_i()
{
  try
    {
      this->do_dispatch();
    }
  catch (const ::CORBA::Exception&)
    {
      // Eat these.  We probably should log these, but since we have already
      // unblocked the requesting thread there is no point in saving it or
      // doing anything with it.
    }
  catch (...)
    {
      // Eat these.  We probably should log these, but since we have already
      // unblocked the requesting thread there is no point in saving it or
      // doing anything with it.
    }
}

void
TAO::CSD::TP_Collocated_Asynch_Request::cancel_i()
{
  this->do_cancel();
}

TAO_END_VERSIONED_NAMESPACE_DECL
