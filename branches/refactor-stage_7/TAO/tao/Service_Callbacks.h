//=============================================================================
/**
 *  @file    Service_Callbacks.h
 *
 *  $Id$
 *
 * This is a generic interface that would be used to override many
 * of the default functionalities that the ORB provides by the
 * services if they have been loaded in to the ORB_Core
 *
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVICE_CALLBACK_H
#define TAO_SERVICE_CALLBACK_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

class TAO_Profile;
class TAO_MProfile;
class TAO_GIOP_Invocation;
class TAO_ServerRequest;
class TAO_Message_State_Factory;
class TAO_Stub;

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

/**
 * @class TAO_Service_Callbacks
 *
 * @brief An Abstract Base class for the hooks in the ORB.
 *
 * This class (would) define all the hooks that may be needed by
 * the ORB to override some of its default behaviour. The methods
 * can be potentially used to call service level specific
 * processing that may be required.
 */
class TAO_Export TAO_Service_Callbacks
{

public:

  /// Dtor
  virtual ~TAO_Service_Callbacks (void);

  /// Select the profile from MProfile as the needs of the services
  /// may be. Return the profile in <pfile>
  virtual CORBA::Boolean select_profile (TAO_MProfile *mprofile,
                                         TAO_Profile *&pfile);

  /// Select the profile from MProfile as the needs of the services
  /// may be. Return the profile in <pfile>
  virtual CORBA::Boolean reselect_profile (TAO_Stub *stub,
                                           TAO_Profile *&pfile);

  /// Reset the profile flags that the services could have
  virtual void reset_profile_flags (void);

  /// Check whether <obj> is nil or not.
  virtual CORBA::Boolean object_is_nil (CORBA::Object_ptr obj);


  /// Check for equivalency of the two profiles
  virtual CORBA::Boolean is_profile_equivalent (const TAO_Profile *,
                                                const TAO_Profile *);

  /// Calculate the hash value..
  virtual CORBA::ULong hash_ft (TAO_Profile *p,
                                CORBA::ULong m);

  /// Allow the service layer to decide whether the COMM_FAILURE
  /// exception should be thrown or a reinvocation is needed
  virtual int raise_comm_failure (TAO_GIOP_Invocation *invoke,
                                  TAO_Profile *profile
                                  ACE_ENV_ARG_DECL);

  /// Allow the service layer to decide whether the TRANSIENT
  /// exception should be thrown or a reinvocation is needed
  virtual int raise_transient_failure (TAO_GIOP_Invocation *invoke,
                                       TAO_Profile *profile
                                       ACE_ENV_ARG_DECL);
};

#if defined (__ACE_INLINE__)
# include "tao/Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_SERVICE_CALLBACK_H*/
