// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Custom_Asynch_Request.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Custom_Asynch_Request.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Custom_Asynch_Request::~TP_Custom_Asynch_Request()
{
}


void
TAO::CSD::TP_Custom_Asynch_Request::dispatch_i()
{
  try
    {
      this->execute_op();
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
TAO::CSD::TP_Custom_Asynch_Request::cancel_i()
{
  this->cancel_op();
}

TAO_END_VERSIONED_NAMESPACE_DECL
