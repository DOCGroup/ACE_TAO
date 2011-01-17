// $Id$

#include "tao/CDR.h"
#include "tao/AnyTypeCode/IOPA.h"
#include "orbsvcs/FtRtEvent/EventChannel/Set_Update_Interceptor.h"
#include "orbsvcs/FTRTC.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Set_Update_Interceptor::TAO_Set_Update_Interceptor ()
: myname_ ("TAO_Set_Update_Interceptor")
{
}

TAO_Set_Update_Interceptor::~TAO_Set_Update_Interceptor (void)
{
}

char *
TAO_Set_Update_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
TAO_Set_Update_Interceptor::destroy (void)
{
}

void
TAO_Set_Update_Interceptor::send_poll (
                                   PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
TAO_Set_Update_Interceptor::send_request (
                                      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  if (ACE_OS::strcmp(operation.in(), "set_update")==0 ||
      ACE_OS::strcmp(operation.in(), "oneway_set_update") ==0)
    {
      CORBA::Any_var a = Request_Context_Repository().get_ft_request_service_context(ri);

      IOP::ServiceContext* sc;

      if ((a.in() >>= sc) ==0)
        return;

      ri->add_request_service_context (*sc, 0);

      FTRT::TransactionDepth transaction_depth =
        Request_Context_Repository().get_transaction_depth(ri);
      TAO_OutputCDR cdr;
      ACE_Message_Block mb;

      if (transaction_depth) {
        if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
          throw CORBA::MARSHAL ();

        // Add Transaction Depth Context
        if ((cdr << transaction_depth) == 0)
          throw CORBA::MARSHAL ();
        sc->context_id = FTRT::FT_TRANSACTION_DEPTH;

        ACE_CDR::consolidate(&mb, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
        sc->context_data.replace(mb.length(), &mb);
#else
        // If the replace method is not available, we will need
        // to do the copy manually.  First, set the octet sequence length.
        CORBA::ULong length = mb.length ();
        sc->context_data.length (length);

        // Now copy over each byte.
        char* base = mb.data_block ()->base ();
        for(CORBA::ULong i = 0; i < length; i++)
          {
            sc->context_data[i] = base[i];
          }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

        ri->add_request_service_context (*sc, 0);

        cdr.reset();
      }

      // Add Sequence Number Context

      FTRT::SequenceNumber sequence_number =
        Request_Context_Repository().get_sequence_number(ri);

      ACE_DEBUG((LM_DEBUG, "send_request : sequence_number = %d\n", sequence_number));
      if (sequence_number != 0) {
        if (!(cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
          throw CORBA::MARSHAL ();
        if ((cdr << sequence_number) == 0)
          throw CORBA::MARSHAL ();
        sc->context_id = FTRT::FT_SEQUENCE_NUMBER;

        ACE_CDR::consolidate(&mb, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
        sc->context_data.replace(mb.length(), &mb);
#else
        // If the replace method is not available, we will need
        // to do the copy manually.  First, set the octet sequence length.
        CORBA::ULong length = mb.length ();
        sc->context_data.length (length);

        // Now copy over each byte.
        char* base = mb.data_block ()->base ();
        for(CORBA::ULong i = 0; i < length; i++)
          {
            sc->context_data[i] = base[i];
          }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

        ri->add_request_service_context (*sc, 0);
      }
    }
}

void
TAO_Set_Update_Interceptor::receive_reply (
  PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Set_Update_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Set_Update_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
