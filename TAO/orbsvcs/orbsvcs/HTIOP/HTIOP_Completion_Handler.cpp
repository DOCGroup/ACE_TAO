#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/HTIOP/HTIOP_Completion_Handler.h"

#include "orbsvcs/HTIOP/HTIOP_Transport.h"
#include "orbsvcs/HTIOP/HTIOP_Endpoint.h"

#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Session.h"

#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Acceptor_Impl.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Completion_Handler::Completion_Handler (ACE_Thread_Manager *t)
  : COMPLETION_BASE(t, nullptr, nullptr),
    orb_core_ (nullptr),
    channel_(nullptr),
    creation_strategy_ (nullptr),
    concurrency_strategy_ (nullptr)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core_ != nullptr);
}

TAO::HTIOP::Completion_Handler::Completion_Handler (TAO_ORB_Core *orb_core,
                                                    CORBA::Boolean )
  :  COMPLETION_BASE(orb_core->thr_mgr(), nullptr, nullptr),
     orb_core_ (orb_core),
     channel_(nullptr),
     creation_strategy_ (nullptr),
     concurrency_strategy_ (nullptr)
{
}


TAO::HTIOP::Completion_Handler::~Completion_Handler ()
{
  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
}

int
TAO::HTIOP::Completion_Handler::open (void*)
{
  this->orb_core_->reactor()->register_handler(this,
                                               ACE_Event_Handler::READ_MASK);

  ACE_NEW_RETURN (creation_strategy_,
                  TAO::HTIOP::CREATION_STRATEGY2 (this->orb_core_),
                  -1);
  ACE_NEW_RETURN (concurrency_strategy_,
                  TAO::HTIOP::CONCURRENCY_STRATEGY2 (this->orb_core_),
                  -1);
  return 0;
}

int
TAO::HTIOP::Completion_Handler::resume_handler ()
{
  return ACE_Event_Handler::ACE_APPLICATION_RESUMES_HANDLER;
}

int
TAO::HTIOP::Completion_Handler::handle_input (ACE_HANDLE h)
{
      // Create a service handler, using the appropriate creation
      // strategy.
  if (this->channel_ == nullptr)
    ACE_NEW_RETURN (this->channel_,
                    ACE::HTBP::Channel (h),
                    -1);

  if (this->channel_->pre_recv() != 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       "TAO::HTIOP::Completion_Handler: pre_recv not done, "
                       "channel state = %d\n",
                       this->channel_->state()),
                      0);

  this->reactor()->remove_handler (this,
                                   ACE_Event_Handler::READ_MASK |
                                   ACE_Event_Handler::DONT_CALL);

  this->channel_->register_notifier(this->reactor());

  // look up session related to the channel.
  // Do we already have a handler for it?
  // If so, get the connection handler, otherwise create it new.

  ACE::HTBP::Session *session = this->channel_->session();
  ACE_Event_Handler *handler = session->handler();

  if (handler == nullptr)
    {
      TAO::HTIOP::Connection_Handler *svc_handler = nullptr;
      if (this->creation_strategy_->make_svc_handler (svc_handler) == -1)
        {
          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO::HTIOP::Completion_Handler %p\n"),
                        ACE_TEXT ("make_svc_handler")));
          return -1;
        }

      svc_handler->peer().session(session);
      session->handler (svc_handler);

      svc_handler->transport()->register_handler();
      svc_handler->open(nullptr);
#if 0
      // *** I am not yet sure how to reconsile the notification strategy
      // with TPC concurrency.

  // Activate the <svc_handler> using the designated concurrency
  // strategy (note that this method becomes responsible for
  // handling errors and freeing up the memory if things go
  // awry...).
      if (this->concurrency_strategy_->
          activate_svc_handler (svc_handler,this->arg_) == -1)
        {
      // Note that <activate_svc_handler> closes the <svc_handler>
      // on failure.

          if (TAO_debug_level > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("activate_svc_handler")));
          return -1;
        }
#endif /* 0 */
    }

  if (this->channel_->state() == ACE::HTBP::Channel::Data_Queued)
    {
      this->reactor()->notify (session->handler(),
                               ACE_Event_Handler::READ_MASK);
    }
  return 0;
}

int
TAO::HTIOP::Completion_Handler::handle_close (ACE_HANDLE,
                                            ACE_Reactor_Mask)
{
  //commit suicide
  delete this;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
