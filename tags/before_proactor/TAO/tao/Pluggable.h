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
//     Fred Kuhns
//
// ============================================================================

#ifndef TAO_PLUGGABLE_H
#define TAO_PLUGGABLE_H

#include "tao/corbafwd.h"
#include "tao/Typecode.h"

class ACE_Addr;
class ACE_Reactor;
class TAO_ORB_Core;

class STUB_Object;
class TAO_Profile;
class TAO_MProfile;
class TAO_Resource_Factory;

// Generic definitions for the new Transport class.
// NOTE, the transport object is created in the Service handler constructor
// and delted in the service handlers destructor!!

class TAO_Export TAO_Transport
{
public:
  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual int idle (void) = 0;
  virtual void close_conn() = 0;
  virtual void resume_conn (ACE_Reactor *reactor) = 0;

  virtual ssize_t send (const ACE_Message_Block *mblk) = 0;
  virtual ssize_t send (const u_char *buf, size_t len) = 0;
  virtual ssize_t send (const iovec *iov, int iovcnt) = 0;

  virtual ssize_t receive (char *buf, size_t len) = 0;
  virtual ssize_t receive (iovec *iov, int iovcnt) = 0;

  virtual TAO_Transport *_nil (void) = 0;
  virtual int is_nil (TAO_Transport *obj) = 0;

  virtual int send_request (TAO_ORB_Core *orb_core, 
                            TAO_OutputCDR &stream, 
                            int twoway) = 0;

//  virtual int send_response (TAO_OutputCDR &response) = 0;

  virtual ACE_HANDLE handle(void) = 0;
  // this is primarily used for error and debugging messages!

  virtual ~TAO_Transport(void);
};

// Generic Profile definitions
class TAO_Export TAO_Profile
{
public:
  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual ASYS_TCHAR *addr_to_string(void) = 0;
  virtual int parse (TAO_InputCDR& cdr, 
                     CORBA::Boolean& continue_decoding, 
                     CORBA::Environment &env) = 0;
  virtual int parse_string (const char *string, CORBA::Environment &env) = 0;
  // Parse the CDR encapsulation in <body>...

  virtual CORBA::TypeCode::traverse_status encode (TAO_OutputCDR *&stream,
                                                   CORBA::Environment &env) = 0;

  virtual CORBA::String to_string (CORBA::Environment &env) = 0;

  virtual const TAO_opaque& body (void) const = 0;
  // The body, an octet sequence that represent the marshaled
  // profile... 

  virtual TAO_ObjectKey *_key (CORBA::Environment &env) = 0;
  virtual const TAO_ObjectKey &object_key (void) const = 0;
  // Obtain the object key, return 0 if the profile cannot be parsed.
  // The memory is owned by this object (not given to the caller).

  virtual ACE_Addr &object_addr (const ACE_Addr *addr) = 0;
  virtual ACE_Addr &object_addr (void) = 0;

  virtual TAO_Transport *transport(void) = 0;
  virtual TAO_Profile *_nil (void) = 0;

  virtual CORBA::ULong hash (CORBA::ULong max,
                             CORBA::Environment &env) = 0;

  virtual CORBA::Boolean is_equivalent (TAO_Profile* other_profile,
                                        CORBA::Environment &env) = 0;

  virtual void reset_hint (void) = 0;
  // this methoid is used with a connection has been reset requiring the
  // hint to be cleaned up and reset to NULL.

  virtual void fwd_profiles (TAO_MProfile *mprofiles) = 0;
  // object will assume ownership for this object!!

  virtual TAO_MProfile *fwd_profiles (void) = 0;
  // this object keeps ownership of this object

  virtual TAO_MProfile *get_fwd_profiles (void) = 0;
  // copy of MProfile, user must delete.

  virtual CORBA::ULong _incr_refcnt (void) = 0;
  virtual CORBA::ULong _decr_refcnt (void) = 0;

protected:
  virtual ~TAO_Profile (void);
  // If you have a virtual method you need a virtual dtor.

};

struct Version {
  CORBA::Octet major;
  CORBA::Octet minor;
  
  Version (const Version &src);
  Version (CORBA::Octet maj = 0, CORBA::Octet min = 0);
  ~Version (void);

  void set_version (CORBA::Octet maj, CORBA::Octet min);
  Version &operator= (const Version &src);
  int operator== (const Version &src);
  int operator== (const Version *&src);

};

// End Profile

class TAO_Export TAO_Acceptor
{
  //   TAOs Abstract Acceptor class used for pluggable protocols.
public:

  virtual TAO_Profile *create_profile (TAO_ObjectKey& object_key) = 0;

  virtual ACE_Event_Handler* acceptor (void) = 0;
  // Return the ACE acceptor...

  virtual CORBA::ULong tag (void) = 0;
  // The tag, each concrete class will have a specific tag value.

  virtual ~TAO_Acceptor (void);

};

// Connector Registry and Generic Connector interface definitions

class TAO_Export TAO_Connector
{
public:

  virtual int preconnect(char* preconnections) = 0;
  virtual int open(TAO_Resource_Factory *trf, ACE_Reactor *reactor) = 0;
  virtual int close(void) = 0;

  virtual CORBA::ULong tag (void) = 0;
  // The tag identifying the specific ORB transport layer
  // protocol.  For example TAO_IOP_TAG_INTERNET_IOP = 0.
  // The tag is used in the IOR to identify the type of profile
  // included. IOR -> {{tag0, profile0} {tag1, profole1} ...}
  // GIOP.h defines typedef CORBA::ULong TAO_IOP_Profile_ID;

  virtual TAO_Transport* connect(TAO_Profile* profile, 
                                 CORBA::Environment &env) = 0;
  // In order to support pluggable we need to abstract away the 
  // connect() method so it can be called from the GIOP code
  // independant of the actual transport protocol in use.

  virtual ~TAO_Connector (void);
  // the destructor

}; 

// How does user add new Protocols?
// Registry needs to decide which profiles are added to profile list.
class TAO_Export TAO_Connector_Registry
{
public:

  TAO_Connector_Registry ();
  ~TAO_Connector_Registry ();
  
  TAO_Connector *get_connector (CORBA::ULong tag);
  CORBA::Boolean add_connector (TAO_Connector *connector);
  // All TAO_Connectors will have a tag() member which will be
  // used for registering object, as well as type checking.

  int open(TAO_Resource_Factory *trf, ACE_Reactor *reactor);
  int close_all(void);

  int preconnect (const char* the_preconnections);

  TAO_Profile *connect (STUB_Object *&obj, CORBA::Environment &env);
  // This is where the transport protocol is selected based on
  // some policy.  This member will call the connect member of the
  // TAO_Connector class which in turn will call the concrete connector.
private:
  TAO_Connector *iiop_connector_;
  // @@ for now this is all we support!  This next iteration will be
  // a bit more generic.  Something like a key, value pair with key
  // equil to the IOP_TYPE and value a pointer to the Connector.
};

#endif  /* TAO_PLUGGABLE_H */
