// $Id$

#include "orbsvcs/Security/CSI_Utils.h"
#include "orbsvcs/CSIC.h"
#include "tao/CDR.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::CSI_Utils::create_sas_service_context (
  const CSI::SASContextBody & sas_context,
  IOP::ServiceContext & sc)
{
  // Marshal CSI::SASContextBody union into an octet sequence suitable
  // for placement in an IOP::ServiceContext.
  // (TAO's compiled marshaling is used for performance reasons.)

  TAO_OutputCDR cdr;
  cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

  cdr << sas_context;

  sc.context_id = IOP::SecurityAttributeService;

  // TAO extension, replace the contents of the octet sequence with
  // the CDR stream.
  const CORBA::ULong len = cdr.total_length ();
  sc.context_data.length (len);
  CORBA::Octet * buf = sc.context_data.get_buffer ();
  for (const ACE_Message_Block * i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }
}

bool
TAO::CSI_Utils::extract_sas_service_context (
  const IOP::ServiceContext & sc,
  CSI::SASContextBody & sas_context)
{
  // Demarshal CSI::SASContextBody union from ServiceContext.
  // (TAO's compiled marshaling is used for performance reasons.)

  TAO_InputCDR cdr (reinterpret_cast<const char*> (
                      sc.context_data.get_buffer ()),
                    sc.context_data.length ());

  ACE_CDR::Boolean byte_order;

  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    return false;

  cdr.reset_byte_order (static_cast<int> (byte_order));

  return (cdr >> sas_context);
}

TAO_END_VERSIONED_NAMESPACE_DECL
