// -*- C++ -*-

#include <openssl/x509.h>

#include "Foo_i.h"

ACE_RCSID (Secure_Invocation,
           Foo_i,
           "$Id$")

Foo_i::Foo_i (CORBA::ORB_ptr orb,
              SecurityLevel2::Current_ptr current)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    current_ (SecurityLevel2::Current::_duplicate (current))
{
}

void
Foo_i::baz (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Foo::Bar::NoSecurityAttributes))
{
  Security::AttributeType desired_attribute;

  desired_attribute.attribute_family.family_definer = 0;

  desired_attribute.attribute_family.family = 1;  // privilege attribute

  desired_attribute.attribute_type = Security::AccessId;

  // Set up the AttributeTypeList that requests the AccessId from the
  // "SecurityCurrent" object.
  Security::AttributeTypeList attribute_type_list;
  attribute_type_list.length (1);
  attribute_type_list[0] = desired_attribute;

  // Get the desired security attributes
  Security::AttributeList_var attribute_list =
    this->current_->get_attributes (attribute_type_list
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If the SecurityLevel1::Current::get_attributes() call above
  // succeeds, then it is likely that some security context
  // information is available for this upcall.  The following code
  // verifies that this is actually the case.

  CORBA::ULong len = attribute_list->length ();
  if (len == 0)
    {
      // The desired security attribute was not available.  This
      // indicates a failure in the underlying security mechanism
      // support.
      ACE_THROW (Foo::Bar::NoSecurityAttributes ());
    }

  // Assume X.509 certificates are in use.
  const char x509[] = "x509";
  Security::OID x509_defining_authority;
  x509_defining_authority.length (sizeof (x509));

  CORBA::Octet *buf =
    x509_defining_authority.get_buffer ();

  ACE_OS_String::memcpy (buf, x509, sizeof (x509));

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      Security::SecAttribute &attribute = attribute_list[i];

      if (attribute.attribute_type.attribute_type == Security::AccessId
          && x509_defining_authority == attribute.defining_authority)
        {
          // Obtain the underlying buffer from the
          // SecAttribute.
          CORBA::Octet *der_cert = attribute.value.get_buffer ();

          char buf[BUFSIZ];

          // Convert the DER encoded X.509 certificate into OpenSSL's
          // internal format.
          X509 *peer = ::d2i_X509 (0,
                                   &der_cert,
                                   attribute.value.length ());

          ::X509_NAME_oneline (::X509_get_issuer_name (peer),
                               buf,
                               BUFSIZ);

          ACE_DEBUG ((LM_DEBUG,
                      "\n"
                      "Security::SecAttribute %u -- Certificate Issuer\n"
                      "===============================================\n"
                      "%s\n",
                      i,
                      buf));

          ::X509_free (peer);
        }
      else
        {
          ACE_DEBUG ((LM_WARNING,
                      "(%P|%t) WARNING: Unknown attribute type <%u> or "
                      "defining authority in SecAttribute %u.\n",
                      attribute.attribute_type,
                      i));
        }
    }
}

void
Foo_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
