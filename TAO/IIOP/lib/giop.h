// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    giop.h
//
// = DESCRIPTION
//     GIOP data structures and support routines
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.
// 
// ============================================================================

#if !defined (TAO_GIOP_H)
#  define TAO_GIOP_H

#  include "ace/OS.h"
#  include "ace/SOCK_Stream.h"
#  if defined (__IIOP_BUILD)
#    include "orb.h"
#    include "stub.h"
#  else
#    include <corba/orb.h>
#    include <corba/stub.h>
#  endif /* __IIOP_BUILD */

#  include "connmgr.h"
#  include "iiopobj.h"		// XXX -- not generic!

// XXX this same typedef is used in other places, e.g. iiopobj.hh
typedef CORBA_SEQUENCE <CORBA_Octet> opaque;

class IOP 
{				// namespace
public:
  // Assigned Protocol/Profile tag values.  ORB protcols may be
  // uniquely identified by tags such as these.  This allows each
  // ORB's own objref profiles to be interchanged using IORs.
  //
  // Email to tag-request@omg.org to allocate tags.

  typedef CORBA_ULong ProfileId;

  enum 
  {
    TAG_INTERNET_IOP = 0,		// IIOP
    TAG_MULTIPLE_COMPONENTS = 1,	// DCE-CIOP

    // This is a subset of the list of other profile tags.
    TAG_ONC_IOP = 0x4f4e4300	// ONC IOP
  };

  struct TaggedProfile 
  // One per protocol.
  {		
    ProfileId tag;
    opaque profile_data;
  };

  typedef CORBA_SEQUENCE <TaggedProfile> TaggedProfileSeq;

  struct IOR 
  // InteroperableObjectReference ... a set of protocol-specific
  // protocol profiles, plus a type ID.  Only one object is denoted by
  // all of this information.  It's OK to delete all profiles except
  // the one for the single protocol actually being used.
  {
    char *type_id;
    TaggedProfileSeq profiles;
  };

  // Some protocols can be factored into a set of optional components.
  // Use of such components is defined by the protocol's
  // specification.
  //
  // Email to tag-request@omg.org to allocate tags.

  typedef CORBA_ULong ComponentId;

  enum 
  {
    // These are all defined by DCE-CIOP in OMG TC document 95-3-10.
    TAG_DCE_STRING_BINDING = 100,	// string binding handle
    TAG_DCE_BINDING_NAME = 101,	// CDS/GDS/... name
    TAG_DCE_NO_PIPES = 102,		// no component data
    TAG_OBJECT_KEY = 10,		// opaque
    TAG_ENDPOINT_ID = 11,		// uuid
    TAG_LOCATION_POLICY = 12	// octet/enum
  };

  // One way to represent multicomponent profiles, e.g. as done by the
  // DCE-CIOP protocol.  One of these gets encapsulated in
  // TaggedProfile::profile_data.  TAG_MULTIPLE_COMPONENTS may be used
  // to represent protocol profiles structured in that way, but
  // protocol-specific tags facilitate simpler scanning of IORs since
  // you can be assured that each profile only has data used within a
  // single ORB protocol.
  struct TaggedComponent 
  {
    ComponentId tag;
    opaque component_data;
  };

  typedef CORBA_SEQUENCE <TaggedComponent> MultipleComponentProfile;
};

class GIOP // namespace
  // = TITLE
  // Note that some symbols in this module are part of the "Internet"
  // Inter-ORB Protocol (IIOP), not the General IOP.  Only addressing
  // information and certain details of connection usage are specific
  // to IIOP; all other protocol details can be reused by ORB protocols
  // that are built atop connection protocols other than TCP.
  //
  // THREADING NOTE:  Threads should never manipulate another thread's
  // invocations.  In this implementation, all data structures used to
  // represent invocations (and parts of them) are owned by the thread
  // which created them.  Multiple threads may make of course concurrent
  // invocations safely, since the GIOP code is reentrant.
{
public:
  struct Version { CORBA_Octet major, minor; };

  // GIOP protocol version information

  enum { MY_MAJOR = 1, MY_MINOR = 0 };	// 1.0

  // All GIOP messages include a header and message type.

  enum MsgType 
  {
    Request = 0, // sent by client
    Reply = 1, // by server
    CancelRequest = 2, // by client
    LocateRequest = 3, // by client
    LocateReply = 4, // by server
    CloseConnection = 5, // by server
    MessageError = 6, // by both
    EndOfFile = 7 // "discovered" by either
  };

  struct MessageHeader 
  {
    CORBA_Char magic [4]; // "GIOP"
    Version giop_version;
    CORBA_Octet byte_order; // 0 = big, 1 = little
    CORBA_Octet message_type; // MsgType above
    CORBA_ULong message_size; // in byte_order!
  };

  // Support for Implicit ORB Service Context

  typedef CORBA_ULong 	ServiceID;

  enum 
  {
    TransactionService = 0

    // More service IDs may be defined by OMG.
    // @@ This is where our RIOP service ID will be defined...
  };
  
  struct ServiceContext 
  {
    ServiceID context_id;
    opaque context_data;
  };

  typedef CORBA_SEQUENCE <ServiceContext> ServiceContextList;

  // = Request, Reply headers

  // @@ It would be very useful to add comments to the fields in each
  // of these structs and enums.
  struct RequestHeader 
  {
    ServiceContextList service_info;
    CORBA_ULong request_id;
    CORBA_Boolean response_expected;
    opaque object_key;
    CORBA_String operation;
    CORBA_Principal_ptr	requesting_principal;
  };

  enum ReplyStatusType 
  {
    NO_EXCEPTION,
    USER_EXCEPTION,
    SYSTEM_EXCEPTION,
    LOCATION_FORWARD
  };

  struct ReplyHeader 
  {
    ServiceContextList service_info;
    CORBA_ULong request_id;
    ReplyStatusType reply_status;
  };

  // = Cancellation -- applies both to Requests and LocateRequests.

  struct CancelRequestHeader 
  {
    CORBA_ULong request_id;
  };

  // = Location service support

  struct LocateRequestHeader 
  {
    CORBA_ULong request_id;
    opaque object_key;
  };

  enum LocateStatusType 
  {
    UNKNOWN_OBJECT,
    OBJECT_HERE,
    OBJECT_FORWARD
  };

  struct LocateReplyHeader 
  {
    CORBA_ULong request_id;
    LocateStatusType locate_status;
  };

  class Invocation 
    // = TITLE
    // Invocation: Sends a Request, optionally reads associated Reply.
    // Uses transport info passed in, doesn't locate anything.
  {
  public:
    Invocation (IIOP_Object *data,
		const char *operation,
		CORBA_Boolean is_roundtrip);

    ~Invocation (void);

    void start (CORBA_Environment &env);
    // <start> goes beyond initialising data structures, and makes
    // calls that may fail -- and thus throw exceptions.

    void put_param (CORBA_TypeCode_ptr tc, 
		    void *value, 
		    CORBA_Environment &env)
      {
	(void) CDR::encoder (tc, value, 0, &stream, env);
      }

    ReplyStatusType invoke (CORBA_ExceptionList &exceptions,
			    CORBA_Environment &env);

    void get_value (CORBA_TypeCode_ptr tc,
		   void *value,
		   CORBA_Environment &env)
      {
	(void) CDR::decoder (tc, value, 0, &stream, env);
      }

    // No CORBA_Context support (deprecated).

  private:
    // @@ Please add comments.
    IIOP_Object *_data;

    const char *opname;

    CORBA_Boolean do_rsvp;

    CORBA_ULong my_request_id;

    ACE_Thread_Mutex lock_;

    u_char buffer [CDR::DEFAULT_BUFSIZE];
    CDR stream;

    TAO_Client_Connection_Handler* handler_;
    // The handler for the client's connection.
#if 0
    autorelease <client_endpoint> endpoint;
#endif
  };

  // = Close a connection, first sending GIOP::CloseConnection
  static void close_connection (ACE_SOCK_Stream &fd, void *ctx);
  static void close_connection (ACE_HANDLE &fd, void *ctx);

  // = Generic server side data dispatch methods

  // These methods are called for all file descriptors on which
  // incoming messages are expected.

  // The <handle_request> routine is used to handle request messages;
  // its <reply> parameter is null if the request is "oneway" (or the
  // client isn't waiting for the response that this request normally
  // creates).
  //
  // The optional <check_forward> routine is used to verify that the
  // request is to be delivered within this process by
  // <handle_request>.  Each call to <handle_request> is preceded by a
  // call to this routine if it's provided.  It's used when handling
  // GIOP "Request" messages as well as GIOP "LocateRequest" messages,
  // and returns an enum to indicate overal status (LocateStatusType)
  // as well as an objref in the case of OBJECT_FORWARD.  That objref
  // is released.
  // 
  // Return: 1==success,0==EOF,-1==error

  typedef LocateStatusType (*ForwardFunc) (opaque &,
					   CORBA_Object_ptr &,
					   void *);

  typedef void (*RequestHandler) (RequestHeader &,
				  CDR &,
				  CDR *,
				  void *,
				  CORBA_Environment &);

  static int incoming_message (ACE_SOCK_Stream &peer,
			       ForwardFunc check_forward,
			       RequestHandler handle_request,
			       void *context,
			       CORBA_Environment &env);

  static int incoming_message (ACE_HANDLE &handle,
			       ForwardFunc check_forward,
			       RequestHandler handle_request,
			       void *context,
			       CORBA_Environment &env);

  static CORBA_Boolean send_message (CDR &stream, 
				     ACE_SOCK_Stream &peer);
  // @@ Can we remove the <ACE_HANDLE> variants soon?
  static CORBA_Boolean send_message (CDR &stream, 
				     ACE_HANDLE &connection);

  // = Reads message, returns message type from header.

  static MsgType read_message (ACE_SOCK_Stream &peer, 
			       CDR &msg, 
			       CORBA_Environment &env);

  static MsgType read_message (ACE_HANDLE &connection,
			       CDR &msg, 
			       CORBA_Environment &env);
};

#endif /* TAO_GIOP_H */
