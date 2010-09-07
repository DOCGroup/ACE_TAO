// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_Request.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_Request.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Collocated_Synch_Request::~TP_Collocated_Synch_Request()
{
}


void
TAO::CSD::TP_Collocated_Synch_Request::dispatch_i()
{
  try
    {
      this->do_dispatch();
    }
  catch (const ::CORBA::Exception& ex)
    {
      // We need to save off a copy of the exception.
      this->exception_ = ex._tao_duplicate();
    }
  catch (...)
    {
      ACE_NEW (this->exception_ ,
               CORBA::UNKNOWN (CORBA::SystemException::_tao_minor_code
                                 (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
                                  CORBA::COMPLETED_MAYBE));
    }

  this->synch_helper_.dispatched();
}

void
TAO::CSD::TP_Collocated_Synch_Request::cancel_i()
{
  this->synch_helper_.cancelled();
}

TAO_END_VERSIONED_NAMESPACE_DECL
