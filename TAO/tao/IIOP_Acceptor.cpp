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

#include "tao/IIOP_Acceptor.h"
#include "tao/IIOP_Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/GIOP.h"

TAO_IIOP_Acceptor::TAO_IIOP_Acceptor (void)
  : TAO_Acceptor (TAO_IOP_TAG_INTERNET_IOP),
    base_acceptor_ ()
{
}

// @@ Fred&Ossama: Maybe not for the current round of changes, but
//    shouldn't the acceptor know which version to create?
//    And isn't this the right place to setup the tagged components of 
//    a v1.[12] profile?

// @@ Fred&Ossama: We need to check this interface: a single
//    TAO_Acceptor may be bound to multiple addresses (think of a
//    multihomed machine with an acceptor listening on the wildcard
//    address), hence the "Right Thing" seems to be that we pass an
//    MProfile that is filled up by the TAO_Acceptor class.

TAO_Profile *
TAO_IIOP_Acceptor::create_profile (TAO_ObjectKey &object_key)
{
  ACE_INET_Addr new_address;

  if (base_acceptor_.acceptor ().get_local_addr (new_address) == -1)
    return 0;

  return new TAO_IIOP_Profile (new_address, object_key);
}

ACE_Event_Handler *
TAO_IIOP_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

int
TAO_IIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_CString &address)
{

  ACE_INET_Addr addr (address.c_str ());

  if (this->base_acceptor_.open (
                  // orb_core->orb_params ()->addr (),
                  addr,
                  orb_core->reactor(),
                  orb_core->server_factory ()->creation_strategy (),
                  orb_core->server_factory ()->accept_strategy (),
                  orb_core->server_factory ()->concurrency_strategy (),
                  orb_core->server_factory ()->scheduling_strategy ()) == -1)
      return -1;
    else {
      // The following step is necessary since the user may have specified
      // a 0 for a port number.  Once we open the acceptor, we can recheck
      // the address and get the accurate port number.
      ACE_INET_Addr new_address;

      if (base_acceptor_.acceptor ().get_local_addr (new_address) == -1)
        return -1;

      // @@ Fred&Ossama: Does this call make any sense now? There is
      //    no such thing as the ORB address anymore, right? And if
      //    this is removed then this whole branch of the if()
      //    statement is a noop.

      // Reset the address
      orb_core->orb_params ()->addr (new_address);

      // iiop_acceptor->acceptor ().enable (ACE_CLOEXEC);
      // this is done in the connection handlers open method.

      // @@  This is broken.  The acceptor registry must be able to
      // determine if a given profile refers to a collocated object.
      // for now, this is done using a hash table and the INET_Addr
      // as the key.  This poa is the value I believe fredk.
      // this->orb_core->add_to_collocation_table ();

      }
  return 0;
}
