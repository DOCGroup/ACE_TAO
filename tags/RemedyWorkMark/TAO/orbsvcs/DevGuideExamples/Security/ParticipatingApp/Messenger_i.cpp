/* -*- C++ -*- $Id$ */

#include "Messenger_i.h"
#include "ace/OS_NS_string.h"
#include <iostream>

Messenger_i::Messenger_i (
   CORBA::ORB_ptr orb,
   SSLIOP::Current_ptr ssliop_current
)
: orb_(CORBA::ORB::_duplicate(orb)),
  ssliop_current_(SSLIOP::Current::_duplicate(ssliop_current))
  {
  }

Messenger_i::~Messenger_i (void)
  {
  }

CORBA::Boolean Messenger_i::send_message (
    const char * user_name,
    const char * subject,
    char *& message
  )
  {
    if (ssliop_current_->no_context())
      std::cout << "Message from:        " << user_name << std::endl;
    else
      std::cout << "SECURE message from: " << user_name << std::endl;

    std::cout << "Subject:             " << subject << std::endl;
    std::cout << "Message:             " << message << std::endl;
    std::cout << std::endl;
    return 1;
  }


void Messenger_i::shutdown (
      const char * user_name
    )
{
  if ( ! (ssliop_current_->no_context()) )
  {
    // requestor is authentic, go ahead and
    // shut the server down.  Report access
    // ID of requestor prior to shutdown.

    std::cout << "Shutdown command from: " << user_name << std::endl;
    std::cout << "Status:                User authenticated." << std::endl;
    std::cout << "Action:                Sever shutdown in progress..." << std::endl;
    std::cout << std::endl;

#if 0
    char name_buf[BUFSIZ];

    //
    // Populate an attribute type list
    // to request the initiating principal's
    // AccessId.
    //
    Security::AttributeTypeList requested_attributes;
    requested_attributes.length(0);
    Security::AttributeType desired_attribute;
    desired_attribute.attribute_family.family_definer = 0;  // OMG
    desired_attribute.attribute_family.family = 1;          // Privilege
                                                            // Attributes
    desired_attribute.attribute_type = Security::AccessId;
    requested_attributes.length(1);
    requested_attributes[0] = desired_attribute;
    //
    // Request the attribtue
    //
    Security::AttributeList_var attrib_list =
      this->current_->get_attributes(requested_attributes);

    if(attrib_list->length() > 0)
    {
      //
      // Copy the values out
      //
      Security::SecAttribute attribute_returned;

      attribute_returned.defining_authority =
         (attrib_list.in())[0].defining_authority ;
      attribute_returned.value =
         (attrib_list.in())[0].value;

      // Certificates are returned in
      // X.509 format
      //
      const char x509[] = "x509";
      //
      // Setup a Security::OID (sequence<octet>)
      // to hold the attribute's defining authority.
      //
      Security::OID x509_defining_authority;
      x509_defining_authority.length(sizeof (x509));
      //
      // Populate the defining authority value.
      //
      CORBA::Octet *buf =
        x509_defining_authority.get_buffer();
      ACE_OS_String::memcpy( buf, x509, sizeof(x509));
      //
      // Confirm the defining authority is "x509".
      //
      if(attribute_returned.defining_authority ==
         x509_defining_authority)
      {
        //
        // Get the buffer holding the certificate
        //
        CORBA::Octet *der_cert =
          attribute_returned.value.get_buffer();
        //
        // Convert the DER encoded certificate into
        // OpenSSL's internal format.
        //
        X509 *peer = ::d2i_X509 (0,
                                 &der_cert,
                                 attribute_returned.value.length());

        ::X509_NAME_oneline(::X509_get_subject_name (peer),
                            name_buf,
                            BUFSIZ);

        ::X509_free(peer);
      }
    }
    //
    // Report the certificate's subject name
    // and terminate the server
    //
    std::cout << "Shutdown commanded by: "
         << name_buf
         << std::endl;
    std::cout << std::endl;
#endif

    orb_->shutdown (0);
  }
  else
  {
    // requestor is not secure,
    // ignore shutdown command
      std::cout << "Shutdown command from: " << user_name << std::endl;
      std::cout << "Status:                User *NOT* authenticated." << std::endl;
      std::cout << "Action:                Ignored." << std::endl;
      std::cout << std::endl;
  }
}
