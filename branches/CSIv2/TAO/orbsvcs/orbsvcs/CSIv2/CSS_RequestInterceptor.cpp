#include "CSS_RequestInterceptor.h"
#include "CSI_Utils.h"


ACE_RCSID (CSIv2,
           CSS_RequestInterceptor,
           "$Id$")


TAO::CSS_RequestInterceptor::CSS_RequestInterceptor (void)
{
}

char *
TAO::CSS_RequestInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO::CSS_RequestInterceptor");
}

void
TAO::CSS_RequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::CSS_RequestInterceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CSI::SASContextBody sas_context;

  const CORBA::Boolean context_created =
    this->get_sas_context_body (info,
                                sas_context
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ec.client_context_id = 0;  // Stateless CSS
  ec.authorization_token = /* Retrieve from ATLAS */;
  ec.identity_token = /* A discriminated union. */;
  ec.client_authentication_token = /* */;

  sas_context.establish_msg (ec);

  // Create IOP::ServiceContext containing CSI::SASContextBody union.
  IOP::ServiceContext sc;

  TAO::CSI_Utils::create_sas_service_context (sas_context, sc);

  const CORBA::boolean replace = 0;

  ri->add_request_service_context (service_context,
                                   replace
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO::CSS_RequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::CSS_RequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CSI::SASContextBody sas_context;

  // Extract CSI::ContextError structure from CSI::SASContextBody.
  const CORBA::Boolean found_context_error =
    this->extract_sas_context (info,
                               sas_context
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If SAS message exists, check if a CSI::CompleteEstablishContext
  // message was sent.
  if (found_sas_context)
    {
      const CSI::MsgType msg_type = sas_context._d ();

      switch (msg_type)
        {
        case CSI::MTCompleteEstablishContext:  // Stateful CSS only.

          const CSI::CompleteEstablishContext & cec =
            sas_context.complete_msg ();

          this->complete_context (cec.client_context_id,
                                  cec.context_stateful);

          break;

        default:
          // No other message types should be sent by a target
          // security service in a normal reply.

          // @@ What do we do in this case?
          break;
        }
    }

}

void
TAO::CSS_RequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::String_var id =
    info->received_exception_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Repository ID of the CORBA::NO_PERMISSION exception
  static const char no_permission[] =
    "IDL:omg.org/CORBA/NO_PERMISSION:1.0";

  // If we received a CORBA::NO_PERMISSION no exception, check if
  // CSI::ContextError ServiceContext was placed in the reply
  // ServiceContextList, and react accordingly.
  if (ACE_OS::strcmp (id.in (), no_permission) == 0)
    {
      CSI::SASContextBody sas_context;

      // Extract CSI::ContextError structure from CSI::SASContextBody.
      const CORBA::Boolean found_sas_context =
        this->extract_sas_context (info,
                                   sas_context
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // If SAS message exists, handle CSI::ContextError messages
      // accordingly.
      if (found_sas_context)
        {
          const CSI::MsgType msg_type = sas_context._d ();

          switch (msg_type)
            {
            case CSI::MTContextError:

              const CSI::ContextError & ce = sas_context.error_msg ();

              if (ce.minor == 1)
                {
                  if (ce.major == 1)       // Invalid evidence
                    {
                      // Re-collect authentication evidence and try
                      // again.
                    }
                  else if (ce.major == 3)  // Conflicting evidence
                    {
                      // Stateful CSS only
                      this->invalidate_context (ce.client_context_id);

                      // Done.  Allow the exception to propagate to
                      // the caller.
                    }
                  else if (ce.major == 4)  // No context
                    {
                      // Stateful CSS only
                      this->invalidate_context (ce.client_context_id);

                      // @@ Restart invocation with new context.
                    }

                  // Don't do anything for the "invalid mechanism"
                  // case (i.e. major == 2).  Just allow the exception
                  // to propagate to the caller.
                }

              break;

            default:
              // No other message types should be sent by a target
              // security service.  Just allow the exception to
              // propagate to the caller.
              break;
            }
        }
    }
}

void
TAO::CSS_RequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // No need to anything for the oneway and LOCATION_FORWARD case.
}

CORBA::Boolean
TAO::TSS_RequestInterceptor::extract_sas_context (
  PortableInterceptor::ServerRequestInfo_ptr info,
  CSI::SASContextBody & sas_context
  ACE_ENV_ARG_DECL)
{
  IOP::ServiceContext_var sc;

  ACE_TRY
    {
      sc = info->get_reply_service_context (IOP::SecurityAttributeService
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 26))
        return 0;
      else
        ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  if (sc->context_id != IOP::SecurityAttributeService)
    {
      // Invalid ServiceId.  The server apparently botched the
      // ServiceContext corresponding to the CSI::SASContextBody.
      //
      // @@ Correct exception?
      ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);
    }

  // Extract CSI::SASContextBody from given IOP::ServiceContext.
  if (!TAO::CSI_Utils::extract_sas_service_context (sc.in (),
                                                    sas_context))
    ACE_THROW_RETURN (CORBA::MARSHAL (), 0);

  return 1;  // Successfully extracted CSI::SASContextBody.
}
