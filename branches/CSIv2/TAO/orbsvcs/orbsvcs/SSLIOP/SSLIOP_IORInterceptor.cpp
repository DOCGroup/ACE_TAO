// $Id$


#include "SSLIOP_IORInterceptor.h"


ACE_RCSID (SSLIOP,
           SSLIOP_IORInterceptor,
           "$Id$")


TAO::SSLIOP::IORInterceptor (TAO_ORB_Core * orb_core,
                             CSIIOP::AssociationOptions csiv2_target_supports,
                             CSIIOP::AssociationOptions csiv2_target_requires)
  : orb_core_ (orb_core),
    csiv2_target_supports_ (csiv2_target_supports),
    csiv2_target_requires_ (csiv2_target_requires)
{
}

char *
TAO::SSLIOP::IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO::SSLIOP::IORInterceptor");
}

void
TAO::SSLIOP::IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Invalidate the TAO_ORB_Core pointer.
  this->orb_core_ = 0;
}

void
TAO::SSLIOP::IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  IOP::TaggedComponent tc;

  tc.tag = IOP::TAG_CSI_SEC_MECH_LIST;

  CSIIOP::CompoundSecMechList component;

  component.stateful = 0;  // Only stateless TSS currently supported.

  CSIIOP::CompoundSecMechanisms & mechanisms = component.mechanism_list;

  // Only SSLIOP is supported at this point in time.
  mechanisms.length (1);

  CSIIOP::CompoundSecMech & mech = mechanisms[0];

  mech.target_requires =
    this->csiv2_target_requires_; // @@ Is this correct?

  if (this->construct_transport_mech_component (mech.transport_mech) != 0)
    ACE_THROW (CORBA::INTERNAL ());

  mech.as_context_mech.target_supports = 0;
  mech.sas_context_mech.target_supports = 0;

  // Marshal the CSIIOP::CompoundSecMechList structure into the
  // TaggedComponent to be embedded in object reference profiles.
  TAO_OutputCDR cdr;

  cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << mech;

  // Replace the contents of the octet sequence with the CDR stream.
  const CORBA::ULong len = cdr.total_length ();
  tc.component_data.length (len);
  const CORBA::Octet * buf = tc.component_data.get_buffer ();
  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  // TAO currently only supports CSIv2 over IIOP.
  info->add_ior_component_to_profile (tc,
                                      IOP::TAG_INTERNET_IOP
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO::SSLIOP::IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::SSLIOP::IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::SSLIOP::IORInterceptor::adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

int
TAO::SSLIOP::IORInterceptor::construct_transport_mech_component (
  IOP::TaggedComponent & tc)
{
  // SSLIOP-specific CSIv2 TaggedComponent information.
  transport_mech.tag = IOP::TAG_TLS_SEC_TRANS;

  CSIIOP::TLS_SEC_TRANS tls_component;

  tls_component.target_supports = this->csiv2_target_supports_;
  tls_component.target_requires = this->csiv2_target_requires_;

  if (this->populate_transport_address_list (tls_component.addresses) != 0)
    return -1;

  // Marshal the CSIIOP::TLS_SEC_TRANS structure into the
  // transport_mech TaggedComponent.
  TAO_OutputCDR cdr;

  cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << tls_component;

  // Replace the contents of the octet sequence with the CDR stream.
  const CORBA::ULong len = cdr.total_length ();
  transport_mech.component_data.length (len);
  const CORBA::Octet * buf = transport_mech.component_data.get_buffer ();
  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  return 0;
}

int
TAO::SSLIOP::IORInterceptor::populate_transport_address_list (
  CSIIOP::TransportAddressList & list)
{
  TAO_Acceptor_Registry & ar =
    this->orb_core_->lane_resources->acceptor_registry ();

  const CORBA::ULong max_endpoints = ar->endpoint_count ();

  // Pre-allocate the maximum size of the transport address list to
  // prevent unnecessary allocations and copying resulting from
  // potential incremental growth.
  list.length (max_endpoints);

  CORBA::ULong i = 0;

  const TAO_AcceptorSetIterator end = this->end ();

  for (TAO_AcceptorSetIterator acceptor = this->begin ();
       acceptor != end;
       ++acceptor)
    {
      if ((*acceptor)->tag () == IOP::TAG_INTERNET_IOP)
        {
          TAO_SSLIOP_Acceptor * s =
            dynamic_cast<TAO_SSLIOP_Acceptor *> (*acceptor);

          ACE_ASSERT (s != 0);

          const ACE_INET_Addr * addrs = s->endpoints ();

          const CORBA::ULong len = s->endpoint_count ();

          for (CORBA::ULong j = 0; j < len; ++j)
            {
              CSIIOP::TransportAddress & address = list[i];
              const ACE_INET_Addr & addr = addrs[j];

              if (this->orb_core_->orb_params ()->use_dotted_decimal_addresses ()
                  && this->dotted_decimal_address (addr,
                                                   address.host_name.out ()) != 0)
                {
                  return -1;
                }
              else
                {
                  address.host_name =
                    CORBA::string_dup (addr.get_host_name ());
                }

              address.port = addr.get_port_number ();

              ++i;
            }
        }
    }

  // Truncate the unused portion of the TransportAddressList.
  if (i < max_endpoints)
    list.length (i);

  return 0;
}
