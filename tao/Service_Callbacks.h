// -*- C++ -*-

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
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVICE_CALLBACK_H
#define TAO_SERVICE_CALLBACK_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include "tao/Invocation_Utils.h"
#include "tao/Object.h"
#include "tao/Service_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;
class TAO_MProfile;
class TAO_Stub;

namespace CORBA
{
  class Object;
  typedef Object *Object_ptr;
}

namespace IOP
{
  class ServiceContextList;
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

  enum Profile_Equivalence
  {
    EQUIVALENT,
    NOT_EQUIVALENT,
    DONT_KNOW
  };


  /// Dtor
  virtual ~TAO_Service_Callbacks (void);

  /// Select the profile from MProfile as the needs of the services
  /// may be. Return the profile in <pfile>
  virtual CORBA::Boolean select_profile (const TAO_MProfile &mprofile,
                                         TAO_Profile *&pfile);

  /// Check whether <obj> is nil or not.
  virtual CORBA::Boolean object_is_nil (CORBA::Object_ptr obj);


  /// Check for equivalency of the two profiles
  virtual Profile_Equivalence is_profile_equivalent (const TAO_Profile *,
                                                     const TAO_Profile *);

  /// Calculate the hash value..
  virtual CORBA::ULong hash_ft (TAO_Profile *p,
                                CORBA::ULong m);

  // Verify if condition is given for LOCATION_FORWARD_PERM reply
  virtual CORBA::Boolean is_permanent_forward_condition (const CORBA::Object_ptr obj,
                                                         const TAO_Service_Context &service_context) const;



  /// Allow the service layer to decide whether the COMM_FAILURE
  /// exception should be thrown or a reinvocation is needed
  virtual TAO::Invocation_Status raise_comm_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile
     );

  /// Allow the service layer to decide whether the TRANSIENT
  /// exception should be thrown or a reinvocation is needed
  virtual TAO::Invocation_Status raise_transient_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile
     );
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_SERVICE_CALLBACK_H*/
