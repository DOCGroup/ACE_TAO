// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Profile.h
//
// = AUTHOR
//   Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PROFILE_H
#define TAO_PROFILE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Tagged_Components.h"
#include "tao/PolicyC.h"
#include "tao/GIOP_Message_State.h"

class TAO_MProfile;
class TAO_Stub;
class TAO_Endpoint;
class TAO_ORB_Core;

class TAO_Export TAO_Profile
{
  // = TITLE
  //   Defines the Profile interface
  //
  // = DESCRIPTION
  //   An abstract base class for representing object location
  //   information.  This is based on the CORBA IOR definitions.
  //
public:

  TAO_Profile (CORBA::ULong tag,
               TAO_ORB_Core *orb_core,
               const TAO_GIOP_Version &version);
  // Constructor

  virtual ~TAO_Profile (void);
  // If you have a virtual method you need a virtual dtor.

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  const TAO_GIOP_Version& version (void) const;
  // Return a pointer to this profile's version.  This object
  // maintains ownership.

  TAO_ORB_Core *orb_core (void) const;
  // Get a poiter to the TAO_ORB_Core

  CORBA::ULong _incr_refcnt (void);
  // Increase the reference count by one on this object.

  CORBA::ULong _decr_refcnt (void);
  // Decrement the object's reference count.  When this count goes to
  // 0 this object will be deleted.

  void forward_to (TAO_MProfile *mprofiles);
  // Keep a pointer to the forwarded profile

  TAO_MProfile* forward_to (void);
  // MProfile accessor

  const TAO_Tagged_Components& tagged_components (void) const;
  TAO_Tagged_Components& tagged_components (void);
  // Access the tagged components, notice that they they could be
  // empty (or ignored) for non-GIOP protocols (and even for GIOP-1.0)

  virtual char object_key_delimiter (void) const = 0;
  // The object key delimiter.

  virtual int parse_string (const char *string,
                            CORBA::Environment &ACE_TRY_ENV) = 0;
  // Initialize this object using the given input string.
  // Supports URL style of object references

  virtual char* to_string (CORBA::Environment &ACE_TRY_ENV) = 0;
  // Return a string representation for this profile.  client must
  // deallocate memory.

  virtual int decode (TAO_InputCDR& cdr) = 0;
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const = 0;
  // Encode this profile in a stream, i.e. marshal it.

  virtual int encode_endpoints (void) = 0;
  // Encodes this profile's endpoints into a tagged component.
  // This is done only if RTCORBA is enabled, since currently this is
  // the only case when we have more than one endpoint per profile.

  virtual const TAO_ObjectKey &object_key (void) const = 0;
  // @@ deprecated. return a reference to the Object Key.

  virtual TAO_ObjectKey *_key (void) const = 0;
  // Obtain the object key, return 0 if the profile cannot be parsed.
  // The memory is owned by the caller!

  virtual TAO_Endpoint *endpoint (void) = 0;
  // Return pointer to this profile's endpoint.  If the profile
  // contains more than one endpoint, i.e., a list, the method returns
  // the head of the list.

  virtual size_t endpoint_count (void) = 0;
  // Return how many endpoints this profile contains.

  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile) = 0;
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their tag, object_key, version and
  // all endpoints are the same.

  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &ACE_TRY_ENV) = 0;
  // Return a hash value for this object.

  virtual IOP::TaggedProfile &create_tagged_profile (void) = 0;
  // This method is used to get the IOP::taggedProfile. The profile
  // information that is received from the server side would have
  // already been decoded. So this method will just make a
  // IOP::TaggedProfile struct from the existing information and
  // return the reference to that. This method is necessary for GIOP
  // 1.2.

  virtual void policies (CORBA::PolicyList *policy_list);
  // This method sets the client exposed policies, i.e., the ones
  // propagated in the IOR, for this profile.

  virtual CORBA::PolicyList&  policies (void);
  // Accessor for the client exposed policies of this profile.

  virtual void the_stub (TAO_Stub *stub);
  // Sets the TAO_Stub to which this profile is associated.

  virtual TAO_Stub* the_stub (void);
  // Gets the TAO_MProfile that holds the TAO_Profile instance.

private:
  TAO_MProfile *forward_to_i (void);
  // this object keeps ownership of this object

  // Profiles should not be copied!
  ACE_UNIMPLEMENTED_FUNC (TAO_Profile (const TAO_Profile&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Profile&))

protected:
  TAO_GIOP_Version version_;
  // IIOP version number.

  TAO_Tagged_Components tagged_components_;
  // The tagged components

  CORBA::Boolean are_policies_parsed_;
  // Flag indicating whether the lazy decoding of the client exposed
  // policies has taken place.

  TAO_Stub *stub_;
  // Pointer to the TAO_Stub to which this profile is related.

  CORBA::PolicyList *policy_list_;
  // Client exposed policies of this profile.

  // NOTE: In this implementation it is assumed that the <policy_list>
  // is exactly the same for each profile.
  // So to save memory, each TAO_Profile has a pointer to the same
  // PolicyList object. The life cycle of this object is managed
  // by the TAO_MProfile class.


private:
  CORBA::ULong tag_;
  // IOP protocol tag.

  TAO_ORB_Core *orb_core_;
  // Pointer to the ORB core

  TAO_MProfile* forward_to_;
  // the TAO_MProfile which contains the profiles for the forwarded
  // object.

  ACE_SYNCH_MUTEX refcount_lock_;
  // Mutex to protect reference count.

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

};

class TAO_Export TAO_Unknown_Profile : public TAO_Profile
{
  // = TITLE
  //   A TAO_Profile class to handle foreign profiles.
  //
  // = DESCRIPTION
  //   The CORBA spec implies that ORBs must be prepared to save and
  //   pass around profiles for protocols it does not recognize. It is
  //   not mandatory to *use* those profiles but they shouldn't be
  //   dropped.
  //   This class stores the information required to marshal and
  //   demarshal an unknown profile, but simply returns an error if
  //   any of the TAO internal methods are invoked.
  //
public:
  TAO_Unknown_Profile (CORBA::ULong tag,
                       TAO_ORB_Core *orb_core);
  // Create the profile

  // = The TAO_Profile methods look above
  virtual int parse_string (const char *string,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual char object_key_delimiter (void) const;
  virtual char* to_string (CORBA::Environment &ACE_TRY_ENV);
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  virtual int encode_endpoints (void);
  virtual const TAO_ObjectKey &object_key (void) const;
  virtual TAO_ObjectKey *_key (void) const;
  virtual TAO_Endpoint *endpoint (void);
  virtual size_t endpoint_count (void);
  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile);
  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual IOP::TaggedProfile &create_tagged_profile (void);

private:
  TAO_opaque body_;
  IOP::TaggedProfile tagged_profile_;
};

#if defined (__ACE_INLINE__)
# include "tao/Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PROFILE_H */
