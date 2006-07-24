#include "tao/Collocated_Invocation.h"
#include "tao/Collocation_Proxy_Broker.h"
#include "tao/ORB_Core.h"
#include "tao/Request_Dispatcher.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Stub.h"
#include "tao/operation_details.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/PortableInterceptorC.h"
#endif /*TAO_HAS_INTERCEPTORS */

ACE_RCSID (tao,
           Collocated_Invocation,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Collocated_Invocation::Collocated_Invocation (CORBA::Object_ptr t,
                                                CORBA::Object_ptr et,
                                                TAO_Stub *stub,
                                                TAO_Operation_Details &detail,
                                                bool response_expected)
    : Invocation_Base (t,
                       et,
                       stub,
                       detail,
                       response_expected,
                       false /* request_is_remote */ )
  {
  }

  Invocation_Status
  Collocated_Invocation::invoke (Collocation_Proxy_Broker *cpb,
                                 Collocation_Strategy strat
                                 ACE_ENV_ARG_DECL)
  {
    Invocation_Status s = TAO_INVOKE_FAILURE;

    /// Start the interception point
#if TAO_HAS_INTERCEPTORS == 1
    s =
      this->send_request_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s != TAO_INVOKE_SUCCESS)
      return s;
#endif /*TAO_HAS_INTERCEPTORS */

    ACE_TRY
      {
        if (strat == TAO_CS_THRU_POA_STRATEGY)
          {
            // Perform invocations on the servant through the servant's ORB.
            CORBA::ORB_var servant_orb =
              this->effective_target ()->_stubobj ()->servant_orb_ptr ();
            TAO_ORB_Core * const orb_core = servant_orb->orb_core ();

            TAO_ServerRequest request (orb_core,
                                       this->details_,
                                       this->effective_target ());

            TAO_Request_Dispatcher * const dispatcher =
              orb_core->request_dispatcher ();

            // Retain ownership of the servant's ORB_Core in case
            // another thread attempts to destroy it (e.g. via
            // CORBA::ORB::destroy()) before this thread complete the
            // invocation.
            orb_core->_incr_refcnt ();
            TAO_ORB_Core_Auto_Ptr my_orb_core (orb_core);

            dispatcher->dispatch (orb_core,
                                  request,
                                  this->forwarded_to_.out ()
                                  ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        else
          {
            cpb->dispatch (this->effective_target (),
                           this->forwarded_to_.out (),
                           this->details_.args (),
                           this->details_.args_num (),
                           this->details_.opname (),
                           this->details_.opname_len (),
                           strat
                           ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }

        // Invocation completed succesfully
        s = TAO_INVOKE_SUCCESS;

#if TAO_HAS_INTERCEPTORS == 1
        if (this->forwarded_to_.in () ||
            this->response_expected_ == false)
          {
            if (this->forwarded_to_.in ())
              this->reply_received (TAO_INVOKE_RESTART);

            s =
              this->receive_other_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        // NOTE: Any other condition that needs handling?
        else if (this->response_expected ())
          {
            this->reply_received (TAO_INVOKE_SUCCESS);

            s =
              this->receive_reply_interception (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
          }
        if (s != TAO_INVOKE_SUCCESS)
          return s;
#endif /*TAO_HAS_INTERCEPTORS */
      }
    ACE_CATCHANY
      {
        // Ignore exceptions for oneways
        if (this->response_expected_ == false)
          return TAO_INVOKE_SUCCESS;

#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus const status =
          this->handle_any_exception (&ACE_ANY_EXCEPTION
                                      ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (status == PortableInterceptor::LOCATION_FORWARD ||
            status == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else
#endif /*TAO_HAS_INTERCEPTORS*/
          ACE_RE_THROW;
      }
# if defined (ACE_HAS_EXCEPTIONS) \
     && defined (ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS)
    ACE_CATCHALL
      {
        if (this->response_expected () == false)
          return TAO_INVOKE_SUCCESS;
#if TAO_HAS_INTERCEPTORS == 1
        PortableInterceptor::ReplyStatus const st =
          this->handle_all_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (st == PortableInterceptor::LOCATION_FORWARD ||
            st == PortableInterceptor::TRANSPORT_RETRY)
          s = TAO_INVOKE_RESTART;
        else
#endif /*TAO_HAS_INTERCEPTORS == 1*/
          ACE_RE_THROW;
      }
# endif  /* ACE_HAS_EXCEPTIONS &&
            ACE_HAS_BROKEN_UNEXPECTED_EXCEPTIONS */
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (this->forwarded_to_.in () != 0)
      s =  TAO_INVOKE_RESTART;

    return s;
  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
