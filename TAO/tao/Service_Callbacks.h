// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Service_Callbacks.h
//
// = DESCRIPTION
//   This is a generic interface that would be used to override many
//   of the default functionalities that the ORB provides.
//
// = AUTHOR
//   Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef TAO_SERVICE_CALLBACK_H
#define TAO_SERVICE_CALLBACK_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Need to figure a way to remove this....
//#include "tao/IOPC.h"

class TAO_Profile;
class TAO_MProfile;

class TAO_Export TAO_Service_Callbacks
{
  // = TITLE
  //   An Abstract Base class for the hooks in the ORB.
  //
  // = DESCRIPTION
  //   This class (would) define all the hooks that may be needed by
  //   the ORB to override some of its default behaviour. The methods
  //   can be potentially used to call service level specific
  //   processing that may be required.

public:

  virtual CORBA::Boolean select_profile (TAO_MProfile *mprofile,
                                         TAO_Profile *&pfile);
  // Select the profile from MProfile as the needs of the services
  // may be. Return the profile in <pfile>

  virtual CORBA::Boolean reselect_profile (TAO_Stub *stub,
                                           TAO_Profile *&pfile);
  // Select the profile from MProfile as the needs of the services
  // may be. Return the profile in <pfile>

  virtual void reset_profile_flags (void);
  // Reset the profile flags that the services could have

  virtual CORBA::Boolean object_is_nil (CORBA::Object_ptr obj);
  // Check whether <obj> is nil or not.
};


#if defined (__ACE_INLINE__)
# include "tao/Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_SERVICE_CALLBACK_H*/
