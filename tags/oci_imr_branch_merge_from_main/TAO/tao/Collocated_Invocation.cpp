#include "Collocated_Invocation.h"
#include "Collocation_Proxy_Broker.h"
#include "operation_details.h"

ACE_RCSID (tao,
           Collocated_Invocation,
           "$Id$")

namespace TAO
{
  Collocated_Invocation::Collocated_Invocation (CORBA::Object_ptr et,
                                                CORBA::Object_ptr t,
                                                TAO_Stub *stub,
                                                TAO_Operation_Details &detail,
                                                bool response_expected)
    : Invocation_Base (et,
                       t,
                       stub,
                       detail,
                       response_expected)
  {
  }

  Invocation_Status
  Collocated_Invocation::invoke (Collocation_Proxy_Broker *cpb,
                                 Collocation_Strategy strat
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
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
        cpb->dispatch (this->effective_target (),
                       this->forwarded_to_.out (),
                       this->details_.args (),
                       this->details_.args_num (),
                       this->details_.opname (),
                       this->details_.opname_len (),
                       strat
                       ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

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
        PortableInterceptor::ReplyStatus status =
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
        PortableInterceptor::ReplyStatus st =
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
            ACE_HAS_BROKEN_UNEXPECTED_EXCEPTION*/
    ACE_ENDTRY;
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (this->forwarded_to_.in () != 0)
      s =  TAO_INVOKE_RESTART;

    return s;
  }

}
