//

#include "SLevel1_Test_i.h"

ACE_RCSID (SecurityLevel1,
           SLevel1_Test_i,
           "$Id$")

SLevel1_Server_i::SLevel1_Server_i (CORBA::ORB_ptr orb,
                                    SecurityLevel1::Current_ptr ss_current)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    ss_current_ (SecurityLevel1::Current::_duplicate (ss_current))
{
}

CORBA::Boolean
SLevel1_Server_i::authorize_level1 (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  /// Lets first set the attributes that we want to get from the
  /// client.
  Security::AttributeType desired_attribute;

  /// Who defines these attributes : OMG
  desired_attribute.attribute_family.family_definer = 0;

  /// What category of attributes do we want: Privilege Attributes
  desired_attribute.attribute_family.family = 1;

  ///  What is the attribute type whose value we want to know:
  ///  AccessId: the identity of the principal used for access
  ///  control
  desired_attribute.attribute_type = Security::AccessId;

  /// Define the AttributeTypeList
  Security::AttributeTypeList attribute_type_list;
  attribute_type_list.length (1);
  attribute_type_list[0] = desired_attribute;

  /// Get the desired security attributes. The in parameter describes
  /// the attribute type/s that we want and the return parameter is the
  /// value/s of the attribute type/s that we want.
  Security::AttributeList_var attribute_list =
    this->ss_current_->get_attributes (attribute_type_list
                                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  /// Just a preliminary check to see if we received something or
  /// not. If we received some value, the length will not be zero.
  CORBA::ULong len = attribute_list->length ();

  if (len == 0)
    {
      // The desired security attribute was not available.  This
      // indicates a failure in the underlying security mechanism
      // support.
      ACE_THROW_RETURN (SLevel1_Server::NoSecurityAttributes (), 1);
    }

  /// Now, that we received the value of our desired attribute, lets
  /// check if the value (in turn, the requesting client) has the
  /// criteria to access this method.
  /// @@  Assume X.509 certificates are in use.
  const char x509[] = "x509";
  Security::OID x509_defining_authority;
  x509_defining_authority.length (sizeof (x509));

  CORBA::Octet *buf =
    x509_defining_authority.get_buffer ();

  ACE_OS_String::memcpy (buf, x509, sizeof (x509));

  X509 *peer;

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      Security::SecAttribute &attribute = attribute_list[i];

      /*
       *  Check if the value that is returned is indeed the
       * Security::AccessId that we requested for and also check if
       * the defining authority is the one that we want... else it is
       * an error.
       */
      if (attribute.attribute_type.attribute_type == Security::AccessId
          && x509_defining_authority == attribute.defining_authority)
        {
          // Obtain the underlying buffer from the
          // SecAttribute.
          CORBA::Octet *der_cert = attribute.value.get_buffer ();

          // Convert the DER encoded X.509 certificate into OpenSSL's
          // internal format.
          peer = ::d2i_X509 (0,
                             &der_cert,
                             attribute.value.length ());

          /// This method creates a dummy certificate that
          /// we will use as the basis for checking the clients
          /// certificate.
          X509 *x = create_check_cert ();

          /// Lets compare the subject name between the certificate
          /// that we received from the client and the certificate we
          /// just created.
          int result = ::X509_subject_name_cmp(peer, x);

          /// Free these X509 pointers as we donot want them anymore.
          ::X509_free (peer);
          ::X509_free (x);

          /// If the subjects match, the client is authorized.
          if (result == 0)
            {
              // Matched and hence authorized: hence return true.
              return 0;
            }
          else
            {
              // Didnt match and hence the client is not authorized.
              return 1;
            }
        }
      else
        {
          ACE_DEBUG ((LM_WARNING,
                      "(%P|%t) WARNING: Unknown attribute type <%u> or "
                      "defining authority in SecAttribute %u.\n",
                      attribute.attribute_type,
                      i));
          return 1;
        }
    }

  // shouldnt come here:
  return 1;
}

X509 *
SLevel1_Server_i::create_check_cert ()
{
  /// Trial to set a name:
  X509 *x = X509_new ();
  X509_NAME *name=NULL;
  X509_NAME_ENTRY *ne=NULL;

  name=X509_NAME_new();

  char *signed_country = ACE_const_cast (char *, "US");
  unsigned char *country = ACE_reinterpret_cast (unsigned char *,
                                                 signed_country);

  char *signed_state = ACE_const_cast (char *, "CA");
  unsigned char *state = ACE_reinterpret_cast (unsigned char *,
                                                 signed_state);

  char *signed_locality = ACE_const_cast (char *, "Irvine");
  unsigned char *locality = ACE_reinterpret_cast (unsigned char *,
                                                 signed_locality);

  char *signed_org = ACE_const_cast (char *, "TAO+OCI");
  unsigned char *org = ACE_reinterpret_cast (unsigned char *,
                                             signed_org);

  char *signed_orgunit = ACE_const_cast (char *, "OCI");
  unsigned char *orgunit = ACE_reinterpret_cast (unsigned char *,
                                                  signed_orgunit);

  char *signed_name = ACE_const_cast (char *, "Priyanka");
  unsigned char *common_name = ACE_reinterpret_cast (unsigned char *,
                                                     signed_name);

  char *signed_email = ACE_const_cast (char *, "pgontla@doc.ece.uci.edu");
  unsigned char *email = ACE_reinterpret_cast (unsigned char *,
                                                 signed_email);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_countryName,
                                   V_ASN1_APP_CHOOSE,country,-1);
  X509_NAME_add_entry(name,ne,0,0);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_stateOrProvinceName,
                                   V_ASN1_APP_CHOOSE,state,-1);
  X509_NAME_add_entry(name,ne,1,0);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_localityName,
                                   V_ASN1_APP_CHOOSE,locality,-1);
  X509_NAME_add_entry(name,ne,2,0);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_organizationName,
                                   V_ASN1_APP_CHOOSE,org,-1);
  X509_NAME_add_entry(name,ne,3,0);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_organizationalUnitName,
                                   V_ASN1_APP_CHOOSE,orgunit,-1);
  X509_NAME_add_entry(name,ne,4,0);

  X509_NAME_ENTRY_create_by_NID(&ne,NID_commonName,
                                V_ASN1_APP_CHOOSE,common_name,-1);
  X509_NAME_add_entry(name,ne,5,0);

  ne=X509_NAME_ENTRY_create_by_NID(NULL,NID_pkcs9_emailAddress,
                                   V_ASN1_APP_CHOOSE,email,-1);
  X509_NAME_add_entry(name,ne,6,0);


  /* finished with structure */
  X509_NAME_ENTRY_free(ne);

  X509_set_subject_name(x,name);
  X509_set_issuer_name(x,name);

  /* finished with structure */
  X509_NAME_free(name);

  return x;
}

void
SLevel1_Server_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
