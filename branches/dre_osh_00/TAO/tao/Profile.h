// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Profile.h
 *
 *  $Id$
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PROFILE_H
#define TAO_PROFILE_H
#include "ace/pre.h"
#include "tao/Tagged_Components.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP_Message_Version.h"
#include "tao/Object_KeyC.h"


#include "ace/Synch.h"

class TAO_MProfile;
class TAO_Stub;
class TAO_Endpoint;
class TAO_ORB_Core;

/// Forward declaration of PolicyList
namespace CORBA
{
  class PolicyList;
}

/**
 * @class TAO_Profile
 *
 * @brief Defines the Profile interface
 *
 * An abstract base class for representing object location
 * information.  This is based on the CORBA IOR definitions.
 */
class TAO_Export TAO_Profile
{
public:

  /// Constructor
  TAO_Profile (CORBA::ULong tag,
               TAO_ORB_Core *orb_core,
               const TAO_GIOP_Message_Version &version);

  /// If you have a virtual method you need a virtual dtor.
  virtual ~TAO_Profile (void);

  /**
   * @name Non virtual methods for the profile classes.
  */

  //@{
  /// The tag, each concrete class will have a specific tag value.
  CORBA::ULong tag (void) const;

  /// Return a pointer to this profile's version.  This object
  /// maintains ownership.
  const TAO_GIOP_Message_Version &version (void) const;

  /// Get a pointer to the <TAO_ORB_Core>.
  TAO_ORB_Core *orb_core (void) const;

  /// Increase the reference count by one on this object.
  CORBA::ULong _incr_refcnt (void);

  /// Decrement the object's reference count.  When this count goes to
  /// 0 this object will be deleted.
  CORBA::ULong _decr_refcnt (void);

  /// Keep a pointer to the forwarded profile
  void forward_to (TAO_MProfile *mprofiles);

  /// MProfile accessor
  TAO_MProfile* forward_to (void);

  /// Access the tagged components, notice that they they could be
  /// empty (or ignored) for non-GIOP protocols (and even for GIOP-1.0)
  const TAO_Tagged_Components& tagged_components (void) const;
  TAO_Tagged_Components& tagged_components (void);

  /// Add the given tagged component to the profile.
  void add_tagged_component (const IOP::TaggedComponent &component
                             ACE_ENV_ARG_DECL);

  /**
   * Return the current addressing mode for this profile.
   * In almost all cases, this is TAO_Target_Specification::Key_Addr.
   */
  CORBA::Short addressing_mode (void) const;

  /// @@ deprecated. return a reference to the Object Key.
  const TAO::ObjectKey &object_key (void) const;

  /// Obtain the object key, return 0 if the profile cannot be parsed.
  /// The memory is owned by the caller!
  TAO::ObjectKey *_key (void) const;
  //@}


  /**
   * @name Template methods that needs to be implemented by the
   * concrete classes. Some of the methods may be overridden only
   * under specila circumstances.
   */
  //@{
  /// Encode this profile in a stream, i.e. marshal it.
  virtual int encode (TAO_OutputCDR &stream) const;

  /// Initialize this object using the given CDR octet string.
  virtual int decode (TAO_InputCDR& cdr);

  /**
   * This method is used to get the IOP::TaggedProfile. The profile
   * information that is received from the server side would have
   * already been decoded. So this method will just make a
   * IOP::TaggedProfile struct from the existing information and
   * return the reference to that. This method is necessary for GIOP
   * 1.2.
   */
  IOP::TaggedProfile *create_tagged_profile (void);

  /// This method sets the client exposed policies, i.e., the ones
  /// propagated in the IOR, for this profile.
  virtual void policies (CORBA::PolicyList *policy_list
                         ACE_ENV_ARG_DECL);

  /// Accessor for the client exposed policies of this profile.
  virtual CORBA::PolicyList&  policies (ACE_ENV_SINGLE_ARG_DECL);

  /// Sets the TAO_Stub to which this profile is associated.
  virtual void the_stub (TAO_Stub *stub);

  /// Gets the TAO_MProfile that holds the TAO_Profile instance.
  virtual TAO_Stub* the_stub (void);

  /// Returns true if this profile can specify multicast endpoints.
  virtual int supports_multicast (void) const;

  /**
   * Set the addressing mode if a remote servant replies with
   * an addressing mode exception.  If this profile doesn't
   * support a particular addressing mode, this method needs to
   * be overridden signal the appropriate error.
   *
   * ** RACE CONDITION NOTE **
   *
   * Currently, getting and setting the addressing mode is not
   * protected by a mutex.  Theoretically, this could cause a race
   * condition if one thread sends a request, then gets an exception
   * from the remote servant to change the addressing mode, and then
   * another thread sends a different request to the same servant
   * using the wrong addressing mode.  The result of this is that
   * we'll get another address change exception.  (Annoying, but not
   * that bad.)
   *
   * In practice at the current time, the above theoretical case
   * never happens since the target specification always uses the
   * object key except for MIOP requests.  Remote ORBs can't respond
   * to MIOP requests even to send exceptions, so even in this case,
   * the race condition can't happen.
   *
   * Therefore, for the time being, there is no lock to protect the
   * addressing mode.  Given that the addressing mode is checked in
   * the critical path, this decision seems like a good thing.
   */
  virtual void addressing_mode (CORBA::Short addr_mode
                                ACE_ENV_ARG_DECL);



  /// The object key delimiter.
  virtual char object_key_delimiter (void) const = 0;

  /// Initialize this object using the given input string.
  /// Supports URL style of object references
  virtual void parse_string (const char *string
                             ACE_ENV_ARG_DECL);

  /// Return a string representation for this profile.  Client must
  /// deallocate memory. Only one endpoint is included into the
  /// string.
  virtual char* to_string (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /**
   * Encodes this profile's endpoints into a tagged component.
   * This is done only if RTCORBA is enabled, since currently this is
   * the only case when we have more than one endpoint per profile.
   */
  virtual int encode_endpoints (void) = 0;



  /**
   * Return pointer to this profile's endpoint.  If the profile
   * contains more than one endpoint, i.e., a list, the method returns
   * the head of the list.
   */
  virtual TAO_Endpoint *endpoint (void) = 0;

  /// Return how many endpoints this profile contains.
  virtual CORBA::ULong endpoint_count (void) = 0;

  /**
   * Return true if this profile is equivalent to other_profile.  Two
   * profiles are equivalent iff their tag, object_key, version and
   * all endpoints are the same.
   */
  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile) = 0;

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (CORBA::ULong max
                             ACE_ENV_ARG_DECL) = 0;
  //@}

protected:
  /**
   * @name Protected template methods.
   */

  //@@{
  /// Decode the protocol specific profile details.
  virtual int decode_profile (TAO_InputCDR &cdr) = 0;

  /// Creates an encapsulation of the ProfileBody struct in the <cdr>
  virtual void create_profile_body (TAO_OutputCDR &cdr) const = 0;

  /**
   * Helper for <decode>.  Decodes endpoints from a tagged component.
   * Decode only if RTCORBA is enabled.  Furthermore, we may not find
   * TAO_TAG_ENDPOINTS component, e.g., if we are talking to nonRT
   * version of TAO or some other ORB.  This is not an error, and we
   * must proceed.  Return 0 on success and -1 on failure.
   */
  virtual int decode_endpoints (void) = 0;

  /// Protocol specific implementation of parse_string ()
  virtual void parse_string_i (const char *string
                               ACE_ENV_ARG_DECL) = 0;
  //@}

  /// To be used by inherited classes
  TAO_Profile (CORBA::ULong tag,
               TAO_ORB_Core *orb_core,
               const TAO::ObjectKey &key,
               const TAO_GIOP_Message_Version &version);

  /// Helper method that encodes the endpoints for RTCORBA as
  /// tagged_components.
  void set_tagged_components (TAO_OutputCDR &cdr);

private:

  /// this object keeps ownership of this object
  TAO_MProfile *forward_to_i (void);

  /// Verify that the current ORB's configuration supports tagged
  /// components in IORs.
  void verify_orb_configuration (ACE_ENV_SINGLE_ARG_DECL);

  /// Verify that the given profile supports tagged components,
  /// i.e. is not a GIOP 1.0 profile.
  void verify_profile_version (ACE_ENV_SINGLE_ARG_DECL);

  // Profiles should not be copied!
  ACE_UNIMPLEMENTED_FUNC (TAO_Profile (const TAO_Profile&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Profile&))

protected:

  /// IIOP version number.
  TAO_GIOP_Message_Version version_;

  /// The tagged components
  TAO_Tagged_Components tagged_components_;

  /// Flag indicating whether the lazy decoding of the client exposed
  /// policies has taken place.
  CORBA::Boolean are_policies_parsed_;

  /// Pointer to the TAO_Stub to which this profile is related.
  TAO_Stub *stub_;

  /// Client exposed policies of this profile.
  CORBA::PolicyList *policy_list_;

  // NOTE: In this implementation it is assumed that the <policy_list>
  // is exactly the same for each profile.
  // So to save memory, each TAO_Profile has a pointer to the same
  // PolicyList object. The life cycle of this object is managed
  // by the TAO_MProfile class.

  /// The current addressing mode.
  /// This may be changed if a remote server sends back an address mode
  /// exception.
  CORBA::Short addressing_mode_;

  /// Our tagged profile
  IOP::TaggedProfile *tagged_profile_;

  /// object_key associated with this profile.
  TAO::ObjectKey object_key_;

private:
  /// IOP protocol tag.
  CORBA::ULong tag_;

  /// Pointer to the ORB core
  TAO_ORB_Core *orb_core_;

  /// the TAO_MProfile which contains the profiles for the forwarded
  /// object.
  TAO_MProfile* forward_to_;

  /// Mutex to protect reference count.
  TAO_SYNCH_MUTEX refcount_lock_;

  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;
};

/**
 * @class TAO_Unknown_Profile
 *
 * @brief A TAO_Profile class to handle foreign profiles.
 *
 * The CORBA spec implies that ORBs must be prepared to save and
 * pass around profiles for protocols it does not recognize. It is
 * not mandatory to *use* those profiles but they shouldn't be
 * dropped.
 * This class stores the information required to marshal and
 * demarshal an unknown profile, but simply returns an error if
 * any of the TAO internal methods are invoked.
 */
class TAO_Export TAO_Unknown_Profile : public TAO_Profile
{
public:
  /// Create the profile
  TAO_Unknown_Profile (CORBA::ULong tag,
                       TAO_ORB_Core *orb_core);

  // = The TAO_Profile methods look above
  virtual void parse_string (const char *string
                             ACE_ENV_ARG_DECL);
  virtual char object_key_delimiter (void) const;
  virtual char* to_string (ACE_ENV_SINGLE_ARG_DECL);
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  virtual int encode_endpoints (void);
  virtual const TAO::ObjectKey &object_key (void) const;
  virtual TAO::ObjectKey *_key (void) const;
  virtual TAO_Endpoint *endpoint (void);
  virtual CORBA::ULong endpoint_count (void);
  virtual CORBA::Boolean is_equivalent (const TAO_Profile* other_profile);
  virtual CORBA::ULong hash (CORBA::ULong max
                             ACE_ENV_ARG_DECL);

  virtual int decode_profile (TAO_InputCDR &cdr);
  virtual int decode_endpoints (void);
private:
  virtual void create_profile_body (TAO_OutputCDR &encap) const;

  virtual void parse_string_i (const char *string
                               ACE_ENV_ARG_DECL);
private:
  TAO_opaque body_;
};

#if defined (__ACE_INLINE__)
# include "Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_PROFILE_H */
