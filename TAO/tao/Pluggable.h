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
#include "tao/Typecode.h"

// Forward declarations.
class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class STUB_Object;
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
  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual void close_connection() = 0;
  // Call the corresponding connection handler's <handle_close>
  // method.
  
  virtual void resume_connection (ACE_Reactor *reactor) = 0;
  // Calls the Reactor's <resume_handler> on behalf of the
  // corresponding connection handler.

  virtual int idle (void) = 0;
  // Idles the corresponding connection handler.

  virtual int is_nil (TAO_Transport *obj) = 0;
  // returns 0 if the obj is 0.

  virtual TAO_Transport *_nil (void) = 0;
  // Return a NULL pointer of type TAO_Transport *.
  
  virtual ACE_HANDLE handle (void) = 0;
  // This is primarily used for error and debugging messages!

  virtual ssize_t send (const ACE_Message_Block *mblk,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of the Message_Block to the connection.

  virtual ssize_t send (const u_char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of the buffer of length len to the connection.

  virtual ssize_t send (const iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  // Write the contents of iovcnt iovec's to the connection.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf.

  virtual ssize_t recv (char *buf,
                        size_t len,
                        int flags,
                        ACE_Time_Value *s = 0) = 0;
  // Read len bytes from into buf using flags.

  virtual ssize_t recv (iovec *iov,
                        int iovcnt,
                        ACE_Time_Value *s = 0) = 0;
  //  Read received data into the iovec buffers.

  virtual int send_request (TAO_ORB_Core *orb_core, 
                            TAO_OutputCDR &stream, 
                            int twoway) = 0;
  // Default action to be taken for send request.

//  virtual int send_response (TAO_OutputCDR &response) = 0;

  virtual ~TAO_Transport (void);
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

class TAO_Export TAO_Profile
{
  // = TITLE
  //   Generic Profile definitions.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual TAO_Transport *transport (void) = 0;
  // Return a pointer to the underlying transport object.  this will
  // provide access to lower layer protocols and processing.

  virtual int parse (TAO_InputCDR& cdr, 
                     CORBA::Boolean& continue_decoding, 
                     CORBA::Environment &env) = 0;
  // Initialize this object using the given CDR octet string.

  virtual int parse_string (const char *string,
                            CORBA::Environment &env) = 0;
  // Initialize this object using the given input string.

  virtual CORBA::String to_string (CORBA::Environment &env) = 0;
  // Return a string representation for this profile.  client must
  // deallocate memory.

  virtual const TAO_opaque &body (void) const = 0;
  // The body, an octet sequence that represent the marshaled
  // profile.

  virtual CORBA::TypeCode::traverse_status encode (TAO_OutputCDR *&stream,
                                                   CORBA::Environment &env) = 0;
  // Encode this profile in a stream, i.e. marshal it.

  virtual const TAO_ObjectKey &object_key (void) const = 0;
  // @@ deprecated. return a reference to the Object Key.

  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ deprecated. set the Object Key.

  virtual TAO_ObjectKey *_key (CORBA::Environment &env) = 0;
  // Obtain the object key, return 0 if the profile cannot be parsed.
  // The memory is owned by this object (not given to the caller).
  
  virtual void forward_to (TAO_MProfile *mprofiles) = 0;
  // object will assume ownership for this object!!

  virtual TAO_MProfile *forward_to (void) = 0;
  // copy of MProfile, user must delete.

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

  virtual ACE_Addr &object_addr (const ACE_Addr *addr) = 0;
  // Set the object_addr for the profile.

  virtual ACE_Addr &object_addr (void) = 0;
  // Return a reference to the object_addr.

  const TAO_IOP_Version *version (void);
  // Return a pointer to this profile's version.  This object
  // maintains ownership.

  const TAO_IOP_Version *version (TAO_IOP_Version *v);
  // First set the version then return a pointer to it.  This object
  // maintains ownership.

  virtual void reset_hint (void) = 0;
  // This method is used with a connection has been reset requiring
  // the hint to be cleaned up and reset to NULL.

  virtual TAO_Profile *_nil (void) = 0;
  // Return a null object pointer.

  virtual CORBA::ULong _incr_refcnt (void) = 0;
  // Increase the reference count by one on this object.

  virtual CORBA::ULong _decr_refcnt (void) = 0;
  // Decrement the object's reference count.  When this count goes to
  // 0 this object will be deleted.

protected:
  virtual TAO_MProfile *forward_to_i (void) = 0;
  // this object keeps ownership of this object

  virtual ~TAO_Profile (void);
  // If you have a virtual method you need a virtual dtor.
};

class TAO_Export TAO_Acceptor
{
  // = TITLE
  //   Abstract Acceptor class used for pluggable protocols.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:

  virtual TAO_Profile *create_profile (TAO_ObjectKey &object_key) = 0;
  // Create the corresponding profile for this endpoint.

  virtual ACE_Event_Handler *acceptor (void) = 0;
  // Return the ACE acceptor...

  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual ~TAO_Acceptor (void);
  // Destructor
};

class TAO_Export TAO_Connector
{
  // = TITLE
  //   Connector Registry and Generic Connector interface definitions.
  //
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  virtual int preconnect (char *preconnections) = 0;
  // Initial set of connections to be established.

  virtual int open (TAO_Resource_Factory *trf,
                    ACE_Reactor *reactor) = 0;
  //  Initialize object and register with reactor.

  virtual int close (void) = 0;
  // Shutdown Connector bridge and concreate Connector. 

  virtual CORBA::ULong tag (void) = 0;
  // The tag identifying the specific ORB transport layer protocol.
  // For example TAO_IOP_TAG_INTERNET_IOP = 0.  The tag is used in the
  // IOR to identify the type of profile included. IOR -> {{tag0,
  // profile0} {tag1, profole1} ...}  GIOP.h defines typedef
  // CORBA::ULong TAO_IOP_Profile_ID;

  virtual TAO_Transport *connect(TAO_Profile *profile,
                                 CORBA::Environment &env) = 0;
  // To support pluggable we need to abstract away the connect()
  // method so it can be called from the GIOP code independant of the
  // actual transport protocol in use.

  virtual ~TAO_Connector (void);
  // the destructor.
}; 

class TAO_Export TAO_Connector_Registry
{
  // = TITLE
  //   All loaded ESIOP or GIOP connector bridges must register with this object.
  // 
  // = DESCRIPTION
  //   @@ Fred, please fill in here.
public:
  TAO_Connector_Registry (void);
  //  Default constructor.

  ~TAO_Connector_Registry (void);
  //  Default destructor. 

  TAO_Connector *get_connector (CORBA::ULong tag);
  // Return the connector bridge corresponding to tag (IOP).

  CORBA::Boolean add_connector (TAO_Connector *connector);
  // All TAO_Connectors will have a tag() member which will be used
  // for registering object, as well as type checking.

  int open (TAO_Resource_Factory *trf,
            ACE_Reactor *reactor);
  // Initialize all registered connectors.

  int close_all (void);
  // Close all open connectors.

  int preconnect (const char *the_preconnections);
  // For this list of preconnections call the connector specific
  // preconnect method for each preconnection.

  TAO_Profile *connect (STUB_Object *&obj,
                        CORBA::Environment &env);
  // This is where the transport protocol is selected based on some
  // policy.  This member will call the connect member of the
  // TAO_Connector class which in turn will call the concrete
  // connector.

private:
  TAO_Connector *iiop_connector_;
  // @@ for now this is all we support!  This next iteration will be a
  // bit more generic.  Something like a key, value pair with key
  // equil to the IOP_TYPE and value a pointer to the Connector.
};

#endif  /* TAO_PLUGGABLE_H */
