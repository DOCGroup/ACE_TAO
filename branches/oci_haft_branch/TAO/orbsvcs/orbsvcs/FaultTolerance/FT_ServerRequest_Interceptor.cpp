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

        ////////////////////////////////////////////////
        // Don't check IOGR version on the pseudo method
        // that pushes a new IOGR
        CORBA::String_var op =
          ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;

        if (ACE_OS::strcmp (op.in (),
          PortableGroup::TAO_UPDATE_OBJECT_GROUP_METHOD_NAME) != 0)
        {
          this->check_iogr_version (sc.in ()
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
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
      PortableGroup::TAO_UPDATE_OBJECT_GROUP_METHOD_NAME) == 0)
    {
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
        if (TAO_debug_level > 0)
          {
            ACE_ERROR ((LM_ERROR,
                        "TAO-FT (%P|%t) - Wrong version information "
                        "within the interceptor [%u | %u] \n",
                        ACE_static_cast( unsigned, fgvsc.object_group_ref_version ),
                        ACE_static_cast( unsigned, this->object_group_ref_version_)
                        ));
          }
      }
    else if (fgvsc.object_group_ref_version <
             this->object_group_ref_version_)
      {
        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_DEBUG,
              "TAO-FT (%P|%t) - Forwarding request to new IOGR  [%u | %u] \n",
                        ACE_static_cast( unsigned, fgvsc.object_group_ref_version ),
                        ACE_static_cast( unsigned, this->object_group_ref_version_)
                        ));
          }
        // Notice that this is a permanent forward.
        ACE_THROW (PortableInterceptor::ForwardRequest (
                   this->iogr_,
                   1));
      }
    else if ((fgvsc.object_group_ref_version ==
             this->object_group_ref_version_) &&
             !this->is_primary_)
      {
        if (TAO_debug_level > 0)
          {
            ACE_DEBUG ((LM_DEBUG,
              "TAO-FT (%P|%t) - Request arrived at backup replica.  Throwing TRANSIENT.[%u] \n",
                        ACE_static_cast( unsigned, this->object_group_ref_version_)
                        ));
          }
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

        // Get an object reference for the ORB's IORManipulation object.
        CORBA::Object_var iorm_obj = this->orb_->resolve_initial_references (
          TAO_OBJID_IORMANIPULATION ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
        this->iorm_ = TAO_IOP::TAO_IOR_Manipulation::_narrow (
          iorm_obj.in () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

      }

    Dynamic::ParameterList_var param =
      ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    if (param->length () != 3)
      ACE_THROW (CORBA::TRANSIENT ());

    const char *str = 0;
    this->is_primary_ = 0;            // assume we're a backup member

    (*param)[0].argument >>= str;                                       // the stringified IOGR
    (*param)[1].argument >>= this->object_group_ref_version_;           // the version
    (*param)[2].argument >>= CORBA::Any::to_boolean (this->is_primary_);// boolean is_primary

    if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
        "TAO-FT (%P|%t) - FT_ServerRequestInterceptor updating IOGR to version %u %s.\n",
        ACE_static_cast (unsigned, this->object_group_ref_version_),
        (this->is_primary_ ? "Primary" : "Backup")
        ));
    }

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
