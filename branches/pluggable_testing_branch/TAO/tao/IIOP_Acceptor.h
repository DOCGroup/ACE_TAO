// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    IIOP_Acceptor.h
//
// = DESCRIPTION
//    IIOP specific acceptor processing
//
// = AUTHOR
//    Fred Kuhns
//
// ============================================================================

#ifndef TAO_IIOP_ACCEPTOR_H
#define TAO_IIOP_ACCEPTOR_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "tao/Pluggable.h"
#include "tao/Connect.h"

// @@ Fred&Ossama: Could we make this a typedef inside the
//    IIOP_Acceptor class?
typedef ACE_Strategy_Acceptor<TAO_Server_Connection_Handler,
                              TAO_SOCK_ACCEPTOR>
        TAO_IIOP_BASE_ACCEPTOR;
// was defined in Connect.h

// TAO IIOP_Acceptor concrete call defination

class TAO_Export TAO_IIOP_Acceptor : public TAO_Acceptor
{
  // = TITLE
  //   TAO_IIOP_Acceptor
  //
  // = DESCRIPTION
  //   The IIOP-specific bridge class for the concrete acceptor.
  //
public:
  // TAO_IIOP_Acceptor (ACE_INET_Addr &addr);
  // Create Acceptor object using addr.

  TAO_IIOP_Acceptor (void);
  // Create Acceptor object using addr.

  int open (TAO_ORB_Core *orb_core, ACE_CString &address);
  // initialize acceptor for this address.

  TAO_Profile *create_profile (TAO_ObjectKey& object_key);
  // create profile object for this Acceptor using the SAP
  // (service access point, Host and Port) and object_key.

  ACE_Event_Handler *acceptor (void);
  // Return the underlying acceptor object, ACE_Acceptor

private:
  TAO_IIOP_BASE_ACCEPTOR base_acceptor_;
  // the concrete acceptor, as a pointer to it's base class.
};

#endif  /* TAO_IIOP_ACCEPTOR_H */
