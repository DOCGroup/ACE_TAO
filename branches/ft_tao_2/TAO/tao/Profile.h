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

class TAO_MProfile;
class TAO_Stub;

class TAO_Export TAO_Profile
{
  // = TITLE
  //   Defines the Profile interface
  //
  // = DESCRIPTION
  //   An abstract base class for representing object address or location
  //   information.  This is based on the CORBA IOR definitions.
  //
public:
  TAO_Profile (CORBA::ULong tag);
  // Constructor

  virtual ~TAO_Profile (void);
  // If you have a virtual method you need a virtual dtor.

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

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

#if (TAO_HAS_FT_CORBA == 1)

  CORBA::Boolean is_primary (void) const;
  // Is this profile a primary?
#endif /*TAO_HAS_FT_CORBA */

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

  virtual const TAO_ObjectKey &object_key (void) const = 0;
  // @@ deprecated. return a reference to the Object Key.

  virtual TAO_ObjectKey *_key (void) const = 0;
  // Obtain the object key, return 0 if the profile cannot be parsed.
  // The memory is owned by the caller!

  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile) = 0;
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their tag, port, host, object_key and
  // version are the same.

  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &ACE_TRY_ENV) = 0;
  // Return a hash value for this object.

  virtual int addr_to_string (char *buffer, size_t length) = 0;
  // Return a string representation for the address.  Returns
  // -1 if buffer is too small.  The purpose of this method is to
  // provide a general interface to the underlying address object's
  // addr_to_string method.  This allows the protocol implementor to
  // select the appropriate string format.

  virtual void reset_hint (void) = 0;
  // This method is used with a connection has been reset requiring
  // the hint to be cleaned up and reset to NULL.

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
  TAO_Unknown_Profile (CORBA::ULong tag);
  // Create the profile

  // = The TAO_Profile methods look above
  virtual int parse_string (const char *string,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual char object_key_delimiter (void) const;
  virtual char* to_string (CORBA::Environment &ACE_TRY_ENV);
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  virtual const TAO_ObjectKey &object_key (void) const;
  virtual TAO_ObjectKey *_key (void) const;
  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile);
  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual int addr_to_string(char *buffer, size_t length);
  virtual void reset_hint (void);
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
