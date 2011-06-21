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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Object_ptr get_target(PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var orb_id = ri->orb_id();

  int argc =0;
  ACE_TCHAR** argv =0;
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, orb_id.in());

  PortableServer::POA_var poa =
    resolve_init<PortableServer::POA>(orb.in(), "RootPOA");

  PortableInterceptor::AdapterName_var adaptor_name =
    ri->adapter_name();

  for (size_t i = 1; i < adaptor_name->length(); ++i) {
    poa = poa->find_POA((*adaptor_name)[i] , false);
  }

  CORBA::OctetSeq_var oid =
    ri->object_id();

  CORBA::Object_var obj =
    poa->id_to_reference(oid.in());

  return obj._retn();
}

CORBA::Object_ptr get_forward(PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::Object_var target =
    get_target(ri);

  TAO::ObjectKey_var key =
    target->_key();

  CORBA::Object_var iogr =
    GroupInfoPublisher::instance()->group_reference();

  CORBA::Object_var forward =
    IOGR_Maker::instance()->ior_replace_key(iogr.in(), key.in());

  return forward._retn();
}


ForwardCtrlServerInterceptor::ForwardCtrlServerInterceptor()
{
}

ForwardCtrlServerInterceptor::~ForwardCtrlServerInterceptor()
{
}

char * ForwardCtrlServerInterceptor::name (void)
{
  return CORBA::string_dup("ForwardCtrlServerInterceptor");
}

void ForwardCtrlServerInterceptor::destroy (void)
{
}

void ForwardCtrlServerInterceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  try{
    IOP::ServiceContext_var service_context =
      ri->get_request_service_context(IOP::FT_GROUP_VERSION);
  }
  catch (const CORBA::Exception&){
    // not an FT call , continue to process the request
    return;
  }

  GroupInfoPublisherBase* publisher = GroupInfoPublisher::instance();
  if (!publisher->is_primary()) {
    // I am not primary, forword the request to primary
    CORBA::Object_var forward = get_forward(ri);

    throw PortableInterceptor::ForwardRequest (forward.in());
  }
}

void ForwardCtrlServerInterceptor::receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr)
{
}

FT::ObjectGroupRefVersion get_ft_group_version(IOP::ServiceContext_var service_context)
{
  Safe_InputCDR cdr (reinterpret_cast<const char*> (service_context->context_data.get_buffer ()),
                      service_context->context_data.length ());

  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::FTGroupVersionServiceContext fgvsc;

  if ((cdr >> fgvsc) == 0)
    throw CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);

  return fgvsc.object_group_ref_version;
}



void ForwardCtrlServerInterceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  IOP::ServiceContext_var service_context;
  FT::ObjectGroupRefVersion version=0;

  try
  {
    if (!ri->response_expected())
      return;

    service_context =
      ri->get_request_service_context(IOP::FT_GROUP_VERSION);
    // get the ref version service context
    version =
      get_ft_group_version(service_context);
  }
  catch (...){
    // not an FT call , continue to reply the request
    return;
  }

  // pass a new IOGR if the client use an outdated version

  IOGR_Maker* maker = IOGR_Maker::instance();
  TAO_FTRTEC::Log(3, ACE_TEXT("Current GROUP Version = %d, received version = %d\n"),
    maker->get_ref_version(), version);

  if (version < maker->get_ref_version()) {
    ACE_DEBUG((LM_DEBUG, "Outdated IOGR version, passing new IOGR\n"));

    try{
      CORBA::Object_var forward = get_forward(ri);

      IOP::ServiceContext sc;
      sc.context_id = FTRT::FT_FORWARD;
      TAO_OutputCDR cdr;

      //if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
      //ACE_THROW (CORBA::MARSHAL ());

      if ((cdr << forward.in() ) == 0 )
        throw CORBA::MARSHAL ();

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

      ri->add_reply_service_context (sc, 0);

      ACE_DEBUG((LM_DEBUG, "reply_service_context added\n"));
    }
    catch (...){
    }
  }

}

void ForwardCtrlServerInterceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void ForwardCtrlServerInterceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
