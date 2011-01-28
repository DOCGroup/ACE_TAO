// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_With_Server_Request.h"
#include "tao/Exception.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_With_Server_Request.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Collocated_Synch_With_Server_Request::~TP_Collocated_Synch_With_Server_Request()
{
}


void
TAO::CSD::TP_Collocated_Synch_With_Server_Request::prepare_for_queue_i()
{
  // NOTE: We *NEED* clone the TAO_ServerRequest for a collocated,
  //       one-way SYNC_WITH_SERVER request.  This is because the
  //       calling thread is signalled just *before* the request is
  //       dispatched. It's (very) possible that the calling thread
  //       will destroy the underlying TAO_ServerRequest object while
  //       the request is dispatching to servant.  This is why we make
  //       a clone - so that we have our own copy that won't be destroyed
  //       while we are using it.
  this->do_clone();
}


void
TAO::CSD::TP_Collocated_Synch_With_Server_Request::dispatch_i()
{
  // This is done *before* we do_dispatch().
  this->synch_helper_.dispatched();

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
TAO::CSD::TP_Collocated_Synch_With_Server_Request::cancel_i()
{
  this->synch_helper_.cancelled();
}

TAO_END_VERSIONED_NAMESPACE_DECL
