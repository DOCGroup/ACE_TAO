#include "TSS_RequestInterceptor.h"
#include "CSI_Utils.h"


ACE_RCSID (CSIv2,
           TSS_RequestInterceptor,
           "$Id$")


char *
TAO::TSS_RequestInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO::TSS_RequestInterceptor");
}

void
TAO::TSS_RequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::TSS_RequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CSI::SASContextBody sas_context;

  // Extract CSI::SASContextBody union from IOP::ServiceContext.
  const CORBA::Boolean found_sas_context =
    this->extract_sas_context (info,
                               sas_context
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If SAS message exists, handle it accordingly.
  if (found_sas_context)
    {
      const CORBA::Boolean oneway =
        info->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      const CSI::MsgType msg_type = sas_context._d ();

      switch (msg_type)
        {
        case CSI::MTEstablishContext:
          CORBA::Boolean stateful;

          if (!this->accept_context (info,
                                     oneway,
                                     sas_context.establish_msg (),
                                     stateful))
            ACE_THROW (CORBA::NO_PERMISSION ());

          // Success!

          // Only add CSI::CompleteEstablishContext to
          // ServiceContextList for twoway calls, i.e those that
          // expect a response.  Don't bother for oneway calls.  This
          // is merely an optimization.
          if (!oneway)
            {
              this->add_complete_establish_context (info,
                                                    ec.client_context_id,
                                                    stateful,
                                                    final_context_token
                                                    ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }

          break;

        case CSI::MTMessageInContext:
          const CSI::MessageInContext & mic = sas_context.in_context_msg ();

          // @@ We should be revalidating/reauthenticating the
          //    identity of the client, right?
          context = this->reference_context (mic.client_context_id);

          // Only add CSI::ContextError to ServiceContextList for
          // twoway calls, i.e those that expect a response.  Don't
          // bother for oneway calls.  This is merely an
          // optimization.
          if (context is empty
              && !oneway)
            {
              // Major and minor codes for "no context" ContextError.
              const CORBA::Long major = 4;
              const CORBA::Long minor = 1;

              this->add_context_error (mic.client_context_id,
                                       major,
                                       minor,
                                       error_token
                                       ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              ACE_THROW (CORBA::NO_PERMISSION ());
            }

          // @@ Should this be done in the send_reply() interception
          //    point?
          // @@ I don't think that's necessary.
          if (mic.discard_context)
            this->discard_context (mic.client_context_id);

          break;

        default:
          // ContextError?
          // No other message types should be sent by a client security
          // service.
          ACE_THROW (CORBA::NO_PERMISSION ());
          break;
        }
    }

  // No SAS message.  Verify that the transport layer security
  // context satisfies the target object security requirements.
  else if (!this->accept_transport_context ())
    ACE_THROW (CORBA::NO_PERMISSION ());

  // Success!
}

void
TAO::TSS_RequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO::TSS_RequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::TSS_RequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO::TSS_RequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

CORBA::Boolean
TAO::TSS_RequestInterceptor::accept_transport_context (void)
{
  /**
   * @todo Query the transport layer.
   */

  return 1; // Context accepted.
}

CORBA::Boolean
TAO::TSS_RequestInterceptor::accept_context (
  PortableInterceptor::ServerRequestInfo_ptr info,
  CORBA::Boolean oneway,
  const CSI::EstablishContext & ec,
  CORBA::Boolean & stateful
  ACE_ENV_ARG_PARAMETER)
{
  ErrorCode error_code = /**/;

  // Only add CSI::ContextError to ServiceContextList for twoway
  // calls, i.e those that expect a response.  Don't bother for oneway
  // calls.  This is merely an optimization.
  if (!oneway && error_code != NONE)
    {
      // The CSI::ContextError major and minor status codes are
      // defined in Section 24.3.5 in Secure Interoperability chapter
      // of the core CORBA specification.
      CORBA::Long major = 0;
      CORBA::Long minor = 1;

      // The "no context" error code should never occur when a context
      // is being established.  It should only occur after a context
      // has been established in a stateful Target Security
      // Service case.
      ACE_ASSERT (error_code != NO_CONTEXT);

      if (error_code == INVALID_EVIDENCE)
        {
          major = 1;
        }
      else if (error_code == CONFLICTING_EVIDENCE)
        {
          major = 2;
        }
      else if (error_code == INVALID_MECHANISM)
        {
          major = 3;
        }
      else if (error_code == POLICY_CHANGE)
        {
          ACE_THROW_RETURN (PortableInterceptor::ForwardRequest (),
                            0);
        }

      this->add_context_error (ec.client_context_id,
                               major,
                               minor,
                               error_token
                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      return 0;
    }

  if (ec.client_context_id == 0)
    stateful = 0;  // Do not establish stateful context for clients
                   // that request a stateless one.
  else
    stateful = 0;  // TAO TSS isn't stateful yet.

  return 1; // Context accepted.
}

void
TAO::TSS_RequestInterceptor::reference_context (CSI::ContextId context_id)
{

}

void
TAO::TSS_RequestInterceptor::discard_context (CSI::ContextId context_id)
{
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
      sc = info->get_request_service_context (IOP::SecurityAttributeService
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
      // Invalid ServiceId.  The client apparently botched the
      // ServiceContext corresponding to the CSI::SASContextBody.
      //
      // @@ Correct exception? If so, should ContextError be added to
      //    the exception reply ServiceContextList.
      ACE_THROW_RETURN (CORBA::NO_PERMISSION (), 0);
    }

  // Extract CSI::SASContextBody from given IOP::ServiceContext.
  if (!TAO::CSI_Utils::extract_sas_service_context (sc.in (),
                                                    sas_context))
    ACE_THROW_RETURN (CORBA::MARSHAL (), 0);

  return 1;  // Successfully extracted CSI::SASContextBody.
}

void
TAO::TSS_RequestInterceptor::add_complete_establish_context (
  PortableInterceptor::ServerRequestInfo_ptr info,
  CSI::ContextId client_context_id,
  CORBA::Boolean context_stateful,
  CSI::GSSToken & final_context_token
  ACE_ENV_ARG_DECL)
{
  CSI::CompleteEstablishContext cec;

  cec.client_context_id = client_context_id;
  cec.context_stateful = context_stateful;

  const CORBA::Boolean release = 0;  // Do not claim ownership of
                                     // final context token sequence
                                     // buffer.

  // To improve performance, shallow copy the final context token
  // octet sequence.  This is fine since the sequence will not be
  // modified.  It will merely be copied into the reply
  // ServiceContextList.
  cec.final_context_token.replace (final_context_token.maximum (),
                                   final_context_token.length (),
                                   final_context_token.get_buffer (),
                                   release);

  CSI::SASContextBody sas_context;
  sas_context.complete_msg (cec);

  this->add_sas_context (info,
                         sas_context
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO::TSS_RequestInterceptor::add_context_error (
  PortableInterceptor::ServerRequestInfo_ptr info,
  CSI::ContextId context_id,
  CORBA::Long major,
  CORBA::Long minor,
  CSI::GSSToken & error_token
  ACE_ENV_ARG_DECL)
{
  CSI::ContextError context_error;

  context_error.client_context_id = context_id;
  context_error.major_status = major;
  context_error.minor_status = minor;

  const CORBA::Boolean release = 0;  // Do not claim ownership of
                                     // error token sequence buffer.

  // To improve performance, shallow copy the error token octet
  // sequence.  This is fine since the sequence will not be modified.
  // It will merely be copied into the reply ServiceContextList.
  context_error.error_token.replace (error_token.maximum (),
                                     error_token.length (),
                                     error_token.get_buffer (),
                                     release);

  CSI::SASContextBody sas_context;
  sas_context.error_msg (context_error);

  this->add_sas_context (info,
                         sas_context
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO::TSS_RequestInterceptor::add_sas_context (
  PortableInterceptor::ServerRequestInfo_ptr info,
  const CSI::SASContextBody & sas_context
  ACE_ENV_ARG_DECL)
{
  // Create IOP::ServiceContext containing CSI::SASContextBody union.
  IOP::ServiceContext sc;

  TAO::CSI_Util::create_sas_service_context (sas, sc);

  // Another IOP::SecurityAttributeService ServiceContext should not
  // exist in the reply's ServiceContextList.
  const CORBA::Boolean replace = 0;

  info->add_reply_service_context (sc,
                                   replace
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
