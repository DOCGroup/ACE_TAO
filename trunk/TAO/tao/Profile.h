// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/Tagged_Components.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/GIOP_Message_Version.h"
#include "tao/Refcounted_ObjectKey.h"
#include "tao/Service_Callbacks.h"
#include "ace/Atomic_Op.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Lock;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_MProfile;
class TAO_Stub;
class TAO_Endpoint;
class TAO_ORB_Core;

namespace CORBA
{
  /// Forward declaration of PolicyList
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

  /**
   * @name Non virtual methods for the profile classes.
  */
  //@{
  /// The tag, each concrete class will have a specific tag value.
  CORBA::ULong tag (void) const;

  /// Return a pointer to this profile's version.  This object
  /// maintains ownership.
  const TAO_GIOP_Message_Version &version (void) const;

  /// Get a pointer to the TAO_ORB_Core.
  TAO_ORB_Core *orb_core (void) const;

  /// Increase the reference count by one on this object.
  unsigned long _incr_refcnt (void);

  /// Decrement the object's reference count.  When this count goes to
  /// 0 this object will be deleted.
  unsigned long _decr_refcnt (void);

  /// Keep a pointer to the forwarded profile
  void forward_to (TAO_MProfile *mprofiles);

  /// MProfile accessor
  TAO_MProfile* forward_to (void);

  /// Access the tagged components, notice that they they could be
  /// empty (or ignored) for non-GIOP protocols (and even for GIOP-1.0)
  const TAO_Tagged_Components& tagged_components (void) const;
  TAO_Tagged_Components& tagged_components (void);

  /// Add the given tagged component to the profile.
  void add_tagged_component (const IOP::TaggedComponent &component);

  /**
   * Return the current addressing mode for this profile.
   * In almost all cases, this is TAO_Target_Specification::Key_Addr.
   */
  CORBA::Short addressing_mode (void) const;

  /// @deprecated Return a reference to the Object Key.
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
  virtual void policies (CORBA::PolicyList *policy_list);

  /// Accessor for the client exposed policies of this profile.
  virtual void  get_policies (CORBA::PolicyList &policy_list);

  /// Returns true if this profile can specify multicast endpoints.
  virtual int supports_multicast (void) const;

  /// Returns true if this profile supports non blocking oneways
  virtual bool supports_non_blocking_oneways (void) const;

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
  virtual void addressing_mode (CORBA::Short addr_mode);

  /// The object key delimiter.
  virtual char object_key_delimiter (void) const = 0;

  /// Initialize this object using the given input string.
  /// Supports URL style of object references
  virtual void parse_string (const char *string);

  /// Return a string representation for this profile.  Client must
  /// deallocate memory. Only one endpoint is included into the
  /// string.
  virtual char* to_string (void) const = 0;

  /**
   * Encodes this profile's endpoints into a tagged component.
   * This is done only if RTCORBA is enabled, since currently this is
   * the only case when we have more than one endpoint per profile.
   */
  virtual int encode_endpoints (void) = 0;

  /**
   * Encodes this profile's endpoints into protocol specific tagged
   * components. This is used for non-RTCORBA applications that share
   * endpoints on profiles. The only known implementation is IIOP, using
   * TAG_ALTERNATE_IIOP_ADDRESS components.
   */
  virtual int encode_alternate_endpoints (void);

  /**
   * Return a pointer to this profile's endpoint.  If the profile
   * contains more than one endpoint, i.e., a list, the method returns
   * the head of the list.
   */
  virtual TAO_Endpoint *endpoint (void) = 0;


  /**
   * Return a pointer to this profile's endpoint.  If the most derived
   * profile type uses an endpoint that is a type that does not derive
   * from the endpoint type of the base profile, then this method returns
   * the base type's endpoint. For example, SSLIOP_Profile derives from
   * IIOP_Profile, but SSLIOP_Endpoint does not derive from IIOP_Endpoint.
   * Because SSLIOP is tagged the same as IIOP, this method is required
   * to facilitate the Endpoint Policy's filtering function.
   * The default implementation of base_endpoint simply returns endpoint.
   */
  virtual TAO_Endpoint *base_endpoint (void);

  /// Return how many endpoints this profile contains.
  virtual CORBA::ULong endpoint_count (void) const = 0;

  /**
   * Return the first endpoint in the list that matches some filtering
   * constraint, such as IPv6 compatibility for IIOP endpoints. This
   * method is implemented in terms of TAO_Endpoint::next_filtered().
   */
  TAO_Endpoint *first_filtered_endpoint (void);

  /// Return the next filtered endpoint in the list after the one
  /// passed in. This method is implemented in terms of
  /// TAO_Endpoint;:next_filtered(). If the supplied source endpoint
  /// is null, this returns the first filtered endpoint.
  TAO_Endpoint *next_filtered_endpoint (TAO_Endpoint *source);

  /**
   * Remove the provided endpoint from the profile. Some
   * subclasses of TAO_Profile already have a protocol-specific
   * version of remove_endpoint, but this generic interface is
   * required. The default implementation is a no-op. Protocol
   * maintainers wishing to add support for the EndpointPolicy must
   * implement remove_generic_endpoint to call their protocol-specific
   * version of remove_endpoint
   */
  virtual void remove_generic_endpoint (TAO_Endpoint *ep);

  /// Add a protocol-agnostic endpoint
  virtual void add_generic_endpoint (TAO_Endpoint *ep);

  /// Verify profile equivalance.
  /**
   * Two profiles are equivalent if their tag, object_key, version
   * and all endpoints are the same.
   *
   * @see do_is_equivalent_i()
   * @see is_equivalent_hook()
   *
   * @return @c true if this profile is equivalent to @c other_profile.
   */
  CORBA::Boolean is_equivalent (const TAO_Profile* other_profile);

  /**
   * Compare the object key for this profile with that of
   * another. This is weaker than is_equivalent
   */
  CORBA::Boolean compare_key (const TAO_Profile *other) const;

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (CORBA::ULong max) = 0;
  //@}

  //@@ TAO_PROFILE_SPL_PUBLIC_METHODS_ADD_HOOK

protected:
  /// If you have a virtual method you need a virtual dtor.
  virtual ~TAO_Profile (void);

  /**
   * @name Protected template methods.
   */
  //@{
  /// Decode the protocol specific profile details.
  virtual int decode_profile (TAO_InputCDR &cdr) = 0;

  /// Creates an encapsulation of the ProfileBody struct in the @a cdr
  virtual void create_profile_body (TAO_OutputCDR &cdr) const = 0;

  /**
   * Helper for decode().  Decodes endpoints from a tagged component.
   * Decode only if RTCORBA is enabled.  Furthermore, we may not find
   * TAO_TAG_ENDPOINTS component, e.g., if we are talking to nonRT
   * version of TAO or some other ORB.  This is not an error, and we
   * must proceed.  Return 0 on success and -1 on failure.
   */
  virtual int decode_endpoints (void) = 0;

  /// Protocol specific implementation of parse_string ()
  virtual void parse_string_i (const char *string) = 0;
  //@}

  /// To be used by inherited classes
  TAO_Profile (CORBA::ULong tag,
               TAO_ORB_Core *orb_core,
               const TAO::ObjectKey &key,
               const TAO_GIOP_Message_Version &version);

  /// Helper method that encodes the endpoints for RTCORBA as
  /// tagged_components.
  void set_tagged_components (TAO_OutputCDR &cdr);

  /// Profile equivalence template method invoked on subclasses.
  /**
   * TAO_Profile subclasses must implement this template method so
   * that they can apply their own definition of profile equivalence.
   */
  virtual CORBA::Boolean do_is_equivalent (const TAO_Profile * other) = 0;

  /// Allow services to apply their own definition of "equivalence."
  /**
   * This method differs from the @c do_is_equivalent() template
   * method in that it has a default implementation that may or not be
   * applicable to all TAO_Profile subclasses.
   */
  virtual TAO_Service_Callbacks::Profile_Equivalence is_equivalent_hook (
                                            const TAO_Profile * other);

  CORBA::ULong hash_service_i (CORBA::ULong m);

private:

  /// This object keeps ownership of this object
  TAO_MProfile *forward_to_i (void);

  /// Verify that the current ORB's configuration supports tagged
  /// components in IORs.
  void verify_orb_configuration (void);

  /// Verify that the given profile supports tagged components,
  /// i.e. is not a GIOP 1.0 profile.
  void verify_profile_version (void);

  // Profiles should not be copied or assigned!
  TAO_Profile (const TAO_Profile&);
  void operator= (const TAO_Profile&);

  //@@ TAO_PROFILE_SPL_PROTECTED_METHODS_ADD_HOOK

protected:

  /// IIOP version number.
  TAO_GIOP_Message_Version version_;

  /// The tagged components
  TAO_Tagged_Components tagged_components_;

  /// Flag indicating whether the lazy decoding of the client exposed
  /// policies has taken place.
  CORBA::Boolean are_policies_parsed_;

  /// The current addressing mode.
  /// This may be changed if a remote server sends back an address mode
  /// exception.
  CORBA::Short addressing_mode_;

  /// Our tagged profile
  IOP::TaggedProfile *tagged_profile_;

  /// Object_key associated with this profile.
  TAO::Refcounted_ObjectKey *ref_object_key_;

private:
  /// IOP protocol tag.
  CORBA::ULong const tag_;

  /// Pointer to the ORB core
  TAO_ORB_Core * const orb_core_;

  /// The TAO_MProfile which contains the profiles for the forwarded
  /// object.
  TAO_MProfile* forward_to_;

  /// Number of outstanding references to this object.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

  /// A lock that protects creation of the tagged profile
  TAO_SYNCH_MUTEX tagged_profile_lock_;

  /// Having (tagged_profile_ != 0) doesn't mean yet that
  /// tagged_profile_ building is finished.
  bool tagged_profile_created_;

  //@@ TAO_PROFILE_SPL_PRIVATE_DATA_ADD_HOOK
};

//@@ TAO_PROFILE_SPL_EXTERN_ADD_HOOK

// A helper class to handle the various kinds of octet sequences used
// inside the ORB.

typedef TAO::unbounded_value_sequence<CORBA::Octet> TAO_opaque;

TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_opaque&);

TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_opaque&);

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
  virtual void parse_string (const char *string);
  virtual char object_key_delimiter (void) const;
  virtual char* to_string (void) const;
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  virtual int encode_endpoints (void);

  virtual TAO::ObjectKey *_key (void) const;
  virtual TAO_Endpoint *endpoint (void);
  virtual CORBA::ULong endpoint_count (void) const;
  virtual CORBA::ULong hash (CORBA::ULong max);

  virtual int decode_profile (TAO_InputCDR &cdr);
  virtual int decode_endpoints (void);

protected:

  virtual CORBA::Boolean do_is_equivalent (const TAO_Profile* other_profile);
  virtual TAO_Service_Callbacks::Profile_Equivalence is_equivalent_hook (
                                           const TAO_Profile* other_profile);

private:
  virtual void create_profile_body (TAO_OutputCDR &encap) const;

  virtual void parse_string_i (const char *string);
private:
  TAO_opaque body_;
};

//@@ TAO_PROFILE_SPL_EXTERN_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Profile.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_PROFILE_H */
