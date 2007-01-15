// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_Request.h"

ACE_RCSID (CSD_ThreadPool,
           TP_Collocated_Synch_Request,
           "$Id$")

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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->do_dispatch(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // We need to save off a copy of the exception.
      this->exception_ = ACE_ANY_EXCEPTION._tao_duplicate();
    }
#if defined (TAO_HAS_EXCEPTIONS)
  ACE_CATCHALL
    {
      ACE_NEW (this->exception_ ,
               CORBA::UNKNOWN (CORBA::SystemException::_tao_minor_code
                                 (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
                                  CORBA::COMPLETED_MAYBE));
    }
#endif
  ACE_ENDTRY;

  this->synch_helper_.dispatched();
}

void
TAO::CSD::TP_Collocated_Synch_Request::cancel_i()
{
  this->synch_helper_.cancelled();
}

TAO_END_VERSIONED_NAMESPACE_DECL
