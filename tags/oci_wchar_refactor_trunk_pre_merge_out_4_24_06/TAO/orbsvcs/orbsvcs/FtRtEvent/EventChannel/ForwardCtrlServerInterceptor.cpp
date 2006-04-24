// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/GroupInfoPublisher.h"
#include "orbsvcs/FtRtEvent/EventChannel/IOGR_Maker.h"
#include "tao/PortableServer/PortableServer.h"
#include "../Utils/resolve_init.h"
#include "../Utils/Safe_InputCDR.h"
#include "../Utils/Log.h"

#include "tao/Object_KeyC.h"
#include "tao/ORB_Constants.h"

#include "orbsvcs/FTRTC.h"

ACE_RCSID (EventChannel,
           ForwardCtrlServerInterceptor,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object_ptr get_target(PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL)
{
  CORBA::String_var orb_id = ri->orb_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  int argc =0;
  char** argv =0;
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, orb_id.in()
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  PortableServer::POA_var poa =
    resolve_init<PortableServer::POA>(orb.in(), "RootPOA"
    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  PortableInterceptor::AdapterName_var adaptor_name =
    ri->adapter_name(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  for (size_t i = 1; i < adaptor_name->length(); ++i) {
    poa = poa->find_POA((*adaptor_name)[i] , false
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(CORBA::Object::_nil());
  }

  CORBA::OctetSeq_var oid =
    ri->object_id(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  CORBA::Object_var obj =
    poa->id_to_reference(oid.in()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  return obj._retn();
}

CORBA::Object_ptr get_forward(PortableInterceptor::ServerRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL)
{
  CORBA::Object_var target =
    get_target(ri ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  TAO::ObjectKey_var key =
    target->_key(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  CORBA::Object_var iogr =
    GroupInfoPublisher::instance()->group_reference();

  CORBA::Object_var forward =
    IOGR_Maker::instance()->ior_replace_key(iogr.in(), key.in()
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(CORBA::Object::_nil());

  return forward._retn();
}


ForwardCtrlServerInterceptor::ForwardCtrlServerInterceptor()
{
}

ForwardCtrlServerInterceptor::~ForwardCtrlServerInterceptor()
{
}

char * ForwardCtrlServerInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup("ForwardCtrlServerInterceptor");
}

void ForwardCtrlServerInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void ForwardCtrlServerInterceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                                    ACE_ENV_ARG_DECL)
                                                    ACE_THROW_SPEC ((CORBA::SystemException,
                                                    PortableInterceptor::ForwardRequest))
{
  ACE_TRY {
    IOP::ServiceContext_var service_context =
      ri->get_request_service_context(IOP::FT_GROUP_VERSION
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY {
    // not an FT call , continue to process the request
    return;
  }
  ACE_ENDTRY;
  ACE_CHECK;

  GroupInfoPublisherBase* publisher = GroupInfoPublisher::instance();
  if (!publisher->is_primary()) {
    // I am not primary, forword the request to primary
    CORBA::Object_var forward = get_forward(ri
      ACE_ENV_ARG_PARAMETER);

    ACE_THROW (PortableInterceptor::ForwardRequest (forward.in()));
  }
}

void ForwardCtrlServerInterceptor::receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
}

FT::ObjectGroupRefVersion get_ft_group_version(IOP::ServiceContext_var service_context
                                               ACE_ENV_ARG_DECL)
{
  Safe_InputCDR cdr (reinterpret_cast<const char*> (service_context->context_data.get_buffer ()),
                      service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO), 0);

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::FTGroupVersionServiceContext fgvsc;

  if ((cdr >> fgvsc) == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 28,
    CORBA::COMPLETED_NO), 0);

  return fgvsc.object_group_ref_version;
}



void ForwardCtrlServerInterceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  IOP::ServiceContext_var service_context;
  FT::ObjectGroupRefVersion version=0;

  ACE_TRY_EX(block1)
  {
    if (!ri->response_expected(ACE_ENV_SINGLE_ARG_PARAMETER))
      return;
    ACE_TRY_CHECK_EX(block1);

    service_context =
      ri->get_request_service_context(IOP::FT_GROUP_VERSION
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);
    // get the ref version service context
    version =
      get_ft_group_version(service_context
      ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(block1);
  }
  ACE_CATCHALL {
    // not an FT call , continue to reply the request
    return;
  }
  ACE_ENDTRY;

  // pass a new IOGR if the client use an outdated version

  IOGR_Maker* maker = IOGR_Maker::instance();
  TAO_FTRTEC::Log(3, "Current GROUP Version = %d, received version = %d\n",
    maker->get_ref_version(), version);

  if (version < maker->get_ref_version()) {
    ACE_DEBUG((LM_DEBUG, "Outdated IOGR version, passing new IOGR\n"));

    ACE_TRY_EX(block2) {
      CORBA::Object_var forward = get_forward(ri
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(block2);

      IOP::ServiceContext sc;
      sc.context_id = FTRT::FT_FORWARD;
      TAO_OutputCDR cdr;

      //if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
      //ACE_THROW (CORBA::MARSHAL ());

      if ((cdr << forward.in() ) == 0 )
        ACE_THROW (CORBA::MARSHAL ());

      ACE_Message_Block mb;
      ACE_CDR::consolidate(&mb, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      sc.context_data.replace(mb.length(), &mb);
#else
      // If the replace method is not available, we will need
      // to do the copy manually.  First, set the octet sequence length.
      CORBA::ULong length = mb.length ();
      sc.context_data.length (length);

      // Now copy over each byte.
      char* base = mb.data_block ()->base ();
      for(CORBA::ULong i = 0; i < length; i++)
        {
          sc.context_data[i] = base[i];
        }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

      ri->add_reply_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX(block2);

      ACE_DEBUG((LM_DEBUG, "reply_service_context added\n"));
    }
    ACE_CATCHALL {
    }
    ACE_ENDTRY;
  }

}

void ForwardCtrlServerInterceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr
                               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
}

void ForwardCtrlServerInterceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr
                           ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
