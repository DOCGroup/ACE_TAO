// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Pluggable.h
//
// = DESCRIPTION
//     Interface for the TAO pluggable protocol frameowrk.
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PLUGGABLE_H
#define TAO_PLUGGABLE_H

#include "tao/corbafwd.h"
#include "tao/Sequence.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class TAO_Stub;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

class TAO_Export TAO_Transport
{
  // = TITLE
  //   Generic definitions for the new Transport class.
  //
  // = DESCRIPTION
  //   The transport object is created in the Service handler
  //   constructor and deleted in the service handlers destructor!!

public:
  TAO_Transport (CORBA::ULong tag);
  // default creator, requres the tag value be supplied.

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  virtual void close_connection() = 0;
  // Call the corresponding connection handler's <handle_close>
  // method.

  virtual void resume_connection (ACE_Reactor *reactor) = 0;
  // Calls the Reactor's <resume_handler> on behalf of the
  // corresponding connection handler.

  virtual int idle (void) = 0;
  // Idles the corresponding connection handler.

  virtual ACE_HANDLE handle (void) = 0;
  // This method provides a way to gain access to the underlying
  // file handle used by the reactor.

  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) = 0;
  // Write the complete Message_Block chain to the connection.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of the buffer of length len to the connection.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of iovcnt iovec's to the connection.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf using flags.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  //  Read received data into the iovec buffers.
  // @@ The ACE_Time_Value *s is just a place holder for now.  It is
  // not clear this this is the best place to specify this.  The actual
  // timeout values will be kept in the Policies.

  virtual int send_request (TAO_ORB_Core *orb_core,
                            TAO_OutputCDR &stream,
                            int twoway) = 0;
  // Default action to be taken for send request.

  virtual ~TAO_Transport (void);
  // destructor

private:
  CORBA::ULong tag_;
  // IOP protocol tag.
};

class TAO_Export TAO_IOP_Version
{
  // = TITLE
  //   Major and Minor version number of the Inter-ORB Protocol.
public:
  CORBA::Octet major;
  // Major version number

  CORBA::Octet minor;
  // Minor version number

  TAO_IOP_Version (const TAO_IOP_Version &src);
  // Copy constructor

  TAO_IOP_Version (CORBA::Octet maj = 0,
           CORBA::Octet min = 0);
  // Default constructor.

  ~TAO_IOP_Version (void);
  // Destructor.

  void set_version (CORBA::Octet maj, CORBA::Octet min);
  // Explicitly set the major and minor version.

  TAO_IOP_Version &operator= (const TAO_IOP_Version &src);
  // Copy operator.

  int operator== (const TAO_IOP_Version &src);
  // Equality operator

  int operator== (const TAO_IOP_Version *&src);
  // Equality operator
};

// @@ Fred&Ossama: We need a *concrete* class (something that can be
//    instantiated) that can be used to represent profiles for
//    protocols we don't know.  This is required in the spec because
//    we are supposed to preserve foreign profiles when communicating
//    with other ORBs.
//    A simple class with noops for most methods and just the basics
//    required for marshaling and demarshaling is what we need.
//
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
  // @@ Fred&Ossama: guys, reference counting *should* be implemented
  //    in the base class, otherwise you are just going to end up
  //    repeating code and forcing the user to implement things not
  //    directly related to protocols.

  void forward_to (TAO_MProfile *mprofiles);
  // Keep a pointer to the forwarded profile

  TAO_MProfile* forward_to (void);
  // MProfile accessor

  virtual int parse_string (const char *string,
                            CORBA::Environment &env) = 0;
  // Initialize this object using the given input string.
  // Supports URL stylr of object references

  virtual CORBA::String to_string (CORBA::Environment &env) = 0;
  // Return a string representation for this profile.  client must
  // deallocate memory.

  virtual const TAO_opaque &body (void) const = 0;
  // The body, an octet sequence that represent the marshaled
  // profile.
  // @@ Fred: We have to think about this method: it basically
  //          requires the profile to keep both the <body> and the
  //          interpreted representation (as host/port/etc.)
  //          This is good for performance reasons, but it may consume
  //          too much memory, maybe a method like this:
  //
  //          void body (TAO_opaque& return_body) const = 0;
  //
  //          will work better.

  virtual int decode (TAO_InputCDR& cdr) = 0;
  // Initialize this object using the given CDR octet string.

  virtual int encode (TAO_OutputCDR &stream) const = 0;
  // Encode this profile in a stream, i.e. marshal it.

  virtual const TAO_ObjectKey &object_key (void) const = 0;
  // @@ deprecated. return a reference to the Object Key.

  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ deprecated. set the Object Key.

  virtual TAO_ObjectKey *_key (CORBA::Environment &env) const = 0;
  // Obtain the object key, return 0 if the profile cannot be parsed.
  // The memory is owned by the caller!

  virtual CORBA::Boolean is_equivalent (TAO_Profile* other_profile,
                                        CORBA::Environment &env) = 0;
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &env) = 0;
  // Return a hash value for this object.

  virtual ASYS_TCHAR *addr_to_string(void) = 0;
  // Return a string representation for the address.
  // @@ Fred: who owns the string returned?

  virtual void reset_hint (void) = 0;
  // This method is used with a connection has been reset requiring
  // the hint to be cleaned up and reset to NULL.

private:
  TAO_MProfile *forward_to_i (void);
  // this object keeps ownership of this object

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
                            CORBA::Environment &env);
  virtual CORBA::String to_string (CORBA::Environment &env);
  virtual const TAO_opaque &body (void) const;
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  virtual const TAO_ObjectKey &object_key (void) const;
  virtual TAO_ObjectKey *_key (CORBA::Environment &env) const;
  virtual CORBA::Boolean is_equivalent (TAO_Profile* other_profile,
                                        CORBA::Environment &env);
  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &env);
  virtual ASYS_TCHAR *addr_to_string(void);
  virtual void reset_hint (void);

private:
  TAO_opaque body_;
};

// ****************************************************************

class TAO_Export TAO_Acceptor
{
  // = TITLE
  //   Abstract Acceptor class used for pluggable protocols.
  //
  // = DESCRIPTION
  //   Base class for the Acceptor bridge calss.
public:

  TAO_Acceptor (CORBA::ULong tag);

  virtual int create_mprofile (const TAO_ObjectKey &object_key,
                               TAO_MProfile &mprofile) = 0;
  // Create the corresponding profile for this endpoint.

  virtual int open (TAO_ORB_Core *orb_core, ACE_CString &address) = 0;
  // method to initialize acceptor for address.

  virtual int open_default (TAO_ORB_Core *orb_core) = 0;
  // Open an acceptor on the default endpoint for this protocol

  virtual ACE_Event_Handler *acceptor (void) = 0;
  // Return the ACE acceptor...

  virtual int is_collocated (const TAO_Profile* profile) = 0;
  // Return 1 if the <profile> has the same endpoint as the acceptor.

  CORBA::ULong tag (void) const;
  // The tag, each concrete class will have a specific tag value.

  virtual CORBA::ULong endpoint_count (void) = 0;
  // returns the number of endpoints this acceptor is listening on.  This
  // is used for determining how many profiles will be generated
  // for this acceptor.

  virtual ~TAO_Acceptor (void);
  // Destructor

private:
  CORBA::ULong tag_;
  // IOP protocol tag.
};

class TAO_Export TAO_Connector
{
  // = TITLE
  //   Generic Connector interface definitions.
  //
  // = DESCRIPTION
  //   Base class for connector bridge object.
public:

  TAO_Connector (CORBA::ULong tag);
  // default constructor.

  virtual ~TAO_Connector (void);
  // the destructor.

  CORBA::ULong tag (void) const;
  // The tag identifying the specific ORB transport layer protocol.
  // For example TAO_IOP_TAG_INTERNET_IOP = 0.  The tag is used in the
  // IOR to identify the type of profile included. IOR -> {{tag0,
  // profile0} {tag1, profole1} ...}  GIOP.h defines typedef
  // CORBA::ULong TAO_IOP_Profile_ID;

  int make_mprofile (const char *ior,
                     TAO_MProfile &mprofile,
                     CORBA::Environment &ACE_TRY_ENV);
  // Parse a string containing a URL style IOR and return an
  // MProfile.

  virtual int open (TAO_ORB_Core *orb_core) = 0;
  //  Initialize object and register with reactor.

  virtual int close (void) = 0;
  // Shutdown Connector bridge and concreate Connector.

  virtual int connect (TAO_Profile *profile,
                       TAO_Transport *&) = 0;
  // To support pluggable we need to abstract away the connect()
  // method so it can be called from the GIOP code independant of the
  // actual transport protocol in use.

  virtual int preconnect (const char *preconnections) = 0;
  // Initial set of connections to be established.

  virtual TAO_Profile *create_profile (TAO_InputCDR& cdr) = 0;
  // Create a profile for this protocol and initialize it based on the
  // encapsulation in <cdr>

protected:
  virtual int make_profile (const char *endpoint,
                            TAO_Profile *&,
                            CORBA::Environment &ACE_TRY_ENV) = 0;
  // Create a profile with a given endpoint.

  virtual int check_prefix (const char *endpoint) = 0;
  // Check that the prefix of the provided endpoint is valid for use
  // with a given pluggable protocol.

  virtual const char object_key_delimiter (void) const = 0;
  // Return the object key delimiter to be used.

private:
  CORBA::ULong tag_;
  // IOP protocol tag.
};

#if defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_PLUGGABLE_H */
