// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#ifndef TAO_ACCEPTOR_H
#  define TAO_ACCEPTOR_H

#include <tao/corba.h>

typedef ACE_Strategy_Acceptor<TAO_Server_Connection_Handler,
                              TAO_SOCK_ACCEPTOR>
        TAO_IIOP_BASE_ACCEPTOR;
// was defined in Conect.h

// TAO IIOP_Acceptor concrete call defination

class TAO_Export TAO_IIOP_Acceptor : public TAO_Acceptor
{
public:
  // TAO_IIOP_Acceptor (ACE_INET_Addr &addr);
  // Create Acceptor object using addr.

  TAO_IIOP_Acceptor (void);
  // Create Acceptor object using addr.

  CORBA::ULong tag (void);
  // The tag, each concrete class will have a specific tag value.

  virtual TAO_Profile *create_profile (TAO_ObjectKey& object_key);
  // create profile object for this Acceptor using the SAP
  // (service access point, Host and Port) and object_key.

  virtual ACE_Event_Handler* acceptor (void);
  // Return the underlying acceptor object, ACE_Acceptor

private:

  TAO_IIOP_BASE_ACCEPTOR base_acceptor_;

  CORBA::ULong tag_;

};

#endif  /* TAO_ACCEPTOR_H */
