// -*- C++ -*-

#include "FT_ServerRequest_Interceptor.h"
#include "tao/IOP_IORC.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

ACE_RCSID (FaultTolerance,
           FT_ServerRequest_Interceptor,
           "$Id$")

namespace TAO
{
  FT_ServerRequest_Interceptor::FT_ServerRequest_Interceptor (void)
    : object_group_ref_version_ (0)
    , is_primary_ (false)
    , iogr_ (0)
    , orb_ (0)
  {
  }

  FT_ServerRequest_Interceptor::~FT_ServerRequest_Interceptor (void)
  {
  }


  char *
  FT_ServerRequest_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup ("FT_ServerRequest_Interceptor");
  }

  void
  FT_ServerRequest_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  FT_ServerRequest_Interceptor::receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    // Check for the group version service context
    ACE_TRY
      {
        IOP::ServiceContext_var sc =
          ri->get_request_service_context (IOP::FT_GROUP_VERSION
                                           ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        this->check_iogr_version (sc.in ()
                                  ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    ACE_CATCH (CORBA::BAD_PARAM, ex)
      {
        // No group version context, no problem just return.
        return;
      }
    ACE_CATCHANY
      {
        ACE_RE_THROW;
      }
    ACE_ENDTRY;
    ACE_CHECK;

  }

  void
  FT_ServerRequest_Interceptor::receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    // Check for the group version service context
    CORBA::String_var op =
      ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (ACE_OS::strcmp (op.in (), 
      "tao_update_object_group") == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        "FT_ServerRequestInterceptor updating IOGR.\n"
        ));
      this->update_iogr (ri
                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    // Else the world is fine
  }

  void
  FT_ServerRequest_Interceptor::send_reply (
      PortableInterceptor::ServerRequestInfo_ptr
      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  FT_ServerRequest_Interceptor::send_exception (
      PortableInterceptor::ServerRequestInfo_ptr
      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
  }

  void
  FT_ServerRequest_Interceptor::send_other (
      PortableInterceptor::ServerRequestInfo_ptr
      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
  }

  void
  FT_ServerRequest_Interceptor::check_iogr_version (
      const IOP::ServiceContext &svc
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                            svc.context_data.get_buffer ()
                                            ),
                      svc.context_data.length ());

    CORBA::Boolean byte_order;

    if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
      return;

    cdr.reset_byte_order (ACE_static_cast (int,byte_order));

    FT::FTGroupVersionServiceContext fgvsc;

    if ((cdr >> fgvsc) == 0)
      ACE_THROW (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28,
                                   CORBA::COMPLETED_NO));


    if (fgvsc.object_group_ref_version >
        this->object_group_ref_version_)
      {
        ACE_ERROR ((LM_ERROR,
                    "TAO-FT (%P|%t) - Wrong version information "
                    "within the interceptor [%u | %u] \n",
                    ACE_static_cast( unsigned, fgvsc.object_group_ref_version ),
                    ACE_static_cast( unsigned, this->object_group_ref_version_)
                    ));
      }
    else if (fgvsc.object_group_ref_version <
             this->object_group_ref_version_)
      {
        ACE_DEBUG ((LM_DEBUG,
          "Forwarding request to new IOGR  [%u | %u] \n",
                    ACE_static_cast( unsigned, fgvsc.object_group_ref_version ),
                    ACE_static_cast( unsigned, this->object_group_ref_version_)
                    ));
        // Notice that this is a permanent forward.
        ACE_THROW (PortableInterceptor::ForwardRequest (
                   this->iogr_,
                   1));
      }
    else if ((fgvsc.object_group_ref_version ==
             this->object_group_ref_version_) &&
             !this->is_primary_)
      {

        ACE_DEBUG ((LM_DEBUG,
          "Request arrived at backup replica.  Throwing TRANSIENT.[%u] \n",
                    ACE_static_cast( unsigned, this->object_group_ref_version_)
                    ));

        ACE_THROW (CORBA::TRANSIENT (
                       CORBA::SystemException::_tao_minor_code (
                       TAO_DEFAULT_MINOR_CODE,
                       EINVAL),
                       CORBA::COMPLETED_NO));
      }
    else
      {
        // Everything is okay...
      }
  }

  void
  FT_ServerRequest_Interceptor::update_iogr (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    if (this->orb_.in () == 0)
      {
        CORBA::String_var orb_id =
          ri->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        int argc = 0;
        char **argv = 0;

        this->orb_ =
          CORBA::ORB_init (argc,
                           argv,
                           orb_id.in ()
                           ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }

    Dynamic::ParameterList_var param =
      ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (param->length () > 2 ||
        param->length () == 0)
      ACE_THROW (CORBA::TRANSIENT ());


    const char *str = 0;


    (*param)[0].argument >>= str;
    (*param)[1].argument >>= this->object_group_ref_version_;

    CORBA::String_var obj (str);

    this->iogr_ =
      this->orb_->string_to_object (obj.in ()
                                    ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;

    // @@ This exception is a hack to let the RM know that we have
    // received and updated the IOGR. We will add a special minor code
    // soon.
    if (this->iogr_.in ())
      ACE_THROW (CORBA::TRANSACTION_ROLLEDBACK ());
  }
}
