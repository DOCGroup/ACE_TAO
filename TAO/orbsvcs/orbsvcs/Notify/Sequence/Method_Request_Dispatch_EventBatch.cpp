// $Id$

#include "Method_Request_Dispatch_EventBatch.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_EventBatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Dispatch_EventBatch, "$id$")

#include "SequencePushConsumer.h"
#include "../Proxy.h"

TAO_NS_Method_Request_Dispatch_EventBatch::TAO_NS_Method_Request_Dispatch_EventBatch (TAO_NS_Event_Collection& event_collection, TAO_NS_SequencePushConsumer* consumer)
  :consumer_ (consumer), event_collection_ (event_collection)
{
  consumer_->_incr_refcnt ();
}

TAO_NS_Method_Request_Dispatch_EventBatch::~TAO_NS_Method_Request_Dispatch_EventBatch ()
{
  consumer_->_decr_refcnt ();
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch_EventBatch::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Dispatch_EventBatch (this->event_collection_, this->consumer_);
}

int
TAO_NS_Method_Request_Dispatch_EventBatch::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->consumer_->proxy ()->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  ACE_TRY
    {
      this->consumer_->push (this->event_collection_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "TAO_NS_Method_Request_Dispatch_EventBatch::: error sending event. ");
      //ACE_RE_THROW;
      }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "TAO_NS_Method_Request_Dispatch_EventBatch:: error sending event. ");
      //ACE_RE_THROW;
    }
  ACE_ENDTRY;

  return 0;
}
