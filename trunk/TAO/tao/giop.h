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

#if 0
#  include "ace/OS.h"
#  include "ace/SOCK_Stream.h"

#  include "tao/orb.h"
#  include "tao/stub.h"

#  include "tao/iiopobj.h"		// XXX -- not generic!
#  include "tao/factories.h"
#endif

// namespace TAO_IOP

// Assigned Protocol/Profile tag values.  ORB protcols may be
// uniquely identified by tags such as these.  This allows each
// ORB's own objref profiles to be interchanged using IORs.
//
// Email to tag-request@omg.org to allocate tags.
typedef CORBA::ULong TAO_IOP_ProfileId;

enum 
{
  TAO_IOP_TAG_INTERNET_IOP = 0, // IIOP
  TAO_IOP_TAG_MULTIPLE_COMPONENTS = 1, // DCE-CIOP

  // This is a subset of the list of other profile tags.
  TAO_IOP_TAG_ONC_IOP = 0x4f4e4300	// ONC IOP
};

struct TAO_IOP_TaggedProfile 
// One per protocol.
{		
  TAO_IOP_ProfileId tag;
  TAO_opaque profile_data;
};

typedef CORBA_SEQUENCE <TAO_IOP_TaggedProfile> TAO_IOP_TaggedProfileSeq;

struct TAO_IOP_IOR 
// InteroperableObjectReference ... a set of protocol-specific
// protocol profiles, plus a type ID.  Only one object is denoted by
// all of this information.  It's OK to delete all profiles except
// the one for the single protocol actually being used.
{
  char *type_id;
  TAO_IOP_TaggedProfileSeq profiles;
};

// Some protocols can be factored into a set of optional components.
  // Use of such components is defined by the protocol's
  // specification.
  //
  // Email to tag-request@omg.org to allocate tags.

typedef CORBA::ULong TAO_IOP_ComponentId;

enum 
{
  // These are all defined by DCE-CIOP in OMG TC document 95-3-10.
  TAO_IOP_TAG_DCE_STRING_BINDING = 100,// string binding handle
  TAO_IOP_TAG_DCE_BINDING_NAME = 101, // CDS/GDS/... name
  TAO_IOP_TAG_DCE_NO_PIPES = 102, // no component data
  TAO_IOP_TAG_OBJECT_KEY = 10,  // opaque
  TAO_IOP_TAG_ENDPOINT_ID = 11, // uuid
  TAO_IOP_TAG_LOCATION_POLICY = 12 // octet/enum
};

  // One way to represent multicomponent profiles, e.g. as done by the
  // DCE-CIOP protocol.  One of these gets encapsulated in
  // TaggedProfile::profile_data.  TAG_MULTIPLE_COMPONENTS may be used
  // to represent protocol profiles structured in that way, but
  // protocol-specific tags facilitate simpler scanning of IORs since
  // you can be assured that each profile only has data used within a
  // single ORB protocol.
struct TAO_IOP_TaggedComponent 
{
  TAO_IOP_ComponentId tag;
  TAO_opaque component_data;
};

typedef CORBA_SEQUENCE <TAO_IOP_TaggedComponent> TAO_IOP_MultipleComponentProfile;

// namespace TAO_GIOP

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
struct TAO_Version
{
  CORBA::Octet major;
  CORBA::Octet minor;
};

// GIOP protocol version information
enum { MY_MAJOR = 1, MY_MINOR = 0 };	// 1.0

// All GIOP messages include a header and message type.
enum TAO_GIOP_MsgType 
{
  TAO_GIOP_Request = 0, // sent by client
  TAO_GIOP_Reply = 1, // by server
  TAO_GIOP_CancelRequest = 2, // by client
  TAO_GIOP_LocateRequest = 3, // by client
  TAO_GIOP_LocateReply = 4, // by server
  TAO_GIOP_CloseConnection = 5, // by server
  TAO_GIOP_MessageError = 6, // by both
  // Not really a message type, but needed to bring that information back somehow
  TAO_GIOP_EndOfFile = 7 // "discovered" by either
};

struct TAO_GIOP_MessageHeader 
{
  CORBA::Char magic [4]; // "GIOP"
  TAO_Version giop_version;
  CORBA::Octet byte_order; // 0 = big, 1 = little
  CORBA::Octet message_type; // MsgType above
  CORBA::ULong message_size; // in byte_order!
};
// defined by GIOP 1.0 protocol
#define	TAO_GIOP_HEADER_LEN sizeof(TAO_GIOP_MessageHeader)


// Support for Implicit ORB Service Context
typedef CORBA::ULong TAO_GIOP_ServiceID;
enum
// = TITLE
// Values for TAO_GIOP_ServiceID
{
  TAO_GIOP_TransactionService = 0

  // More service IDs may be defined by OMG.
    
  // This is where our RIOP service ID will be defined...
};
  
struct TAO_GIOP_ServiceContext 
{
  TAO_GIOP_ServiceID context_id;
  TAO_opaque context_data;
};

typedef CORBA_SEQUENCE <TAO_GIOP_ServiceContext> TAO_GIOP_ServiceContextList;
extern CORBA::TypeCode TC_ServiceContextList;

// = Request, Reply headers

class TAO_GIOP_RequestHeader 
// = TITLE
// This class embodies the header of a GIOP request.
//
// = NOTES
// It used to be a <struct>, which is why everything is public.
{
public:
  TAO_GIOP_RequestHeader (void);
  // Constructor.
    
  CORBA::Boolean init (CDR &msg, CORBA::Environment& env);
  // Initialize the header from the values found in <msg>.
    
  //private:
  TAO_GIOP_ServiceContextList service_info;
  // The service context for the request (CORBA Reference?)
    
  CORBA::ULong request_id;
  // Unique identifier for a request
    
  CORBA::Boolean response_expected;
  // true if this request requires a response
    
  TAO_opaque object_key;
  // @@ the object key of the destination object (is this right?)
    
  CORBA::String operation;
  // Name of the operation being performed

  CORBA::Principal_ptr requesting_principal;
  // Identifies the requester
};

enum TAO_GIOP_ReplyStatusType 
{
  TAO_GIOP_NO_EXCEPTION,                 // Request completed successfully
  TAO_GIOP_USER_EXCEPTION,               // Request terminated with user exception
  TAO_GIOP_SYSTEM_EXCEPTION,             // Request terminated with system exception
  TAO_GIOP_LOCATION_FORWARD              // @@ More info
};

struct TAO_GIOP_ReplyHeader 
{
  TAO_GIOP_ServiceContextList service_info; // Information 
  CORBA::ULong request_id;      // Unique identifier of the request for which this is a reply
  TAO_GIOP_ReplyStatusType reply_status; // Status of the reply (see above enum)
};

// = Cancellation -- applies both to Requests and LocateRequests.
struct TAO_GIOP_CancelRequestHeader 
{
  CORBA::ULong request_id;      // Unique identifier of the request being cancelled
};

// = Location service support
struct TAO_GIOP_LocateRequestHeader 
{
  CORBA::ULong request_id;
  TAO_opaque object_key;
};

enum TAO_GIOP_LocateStatusType 
{
  TAO_GIOP_UNKNOWN_OBJECT,
  TAO_GIOP_OBJECT_HERE,
  TAO_GIOP_OBJECT_FORWARD
};

struct TAO_GIOP_LocateReplyHeader 
{
  CORBA::ULong request_id;
  TAO_GIOP_LocateStatusType locate_status;
};

class TAO_GIOP_Invocation 
// = TITLE
// Invocation: Sends a Request, optionally reads associated Reply.
// Uses transport info passed in, doesn't locate anything.
//
// = NOTES
// This should maybe be moved into its own file (invoke.h?)
{
public:
  TAO_GIOP_Invocation (IIOP_Object *data,
                       const char *operation,
                       CORBA::Boolean is_roundtrip);

  ~TAO_GIOP_Invocation (void);

  void start (CORBA::Environment &env);
  // <start> goes beyond initialising data structures, and makes
  // calls that may fail -- and thus throw exceptions.

  void put_param (CORBA::TypeCode_ptr tc, 
                  void *value,
                  CORBA::Environment &env);

  TAO_GIOP_ReplyStatusType invoke (CORBA::ExceptionList &exceptions,
                                   CORBA::Environment &env);

  void get_value (CORBA::TypeCode_ptr tc,
                  void *value,
                  CORBA::Environment &env);
  // No CORBA::Context support (deprecated).

private:
  // @@ Please add comments.
  IIOP_Object *data_;

  const char *opname;

  CORBA::Boolean do_rsvp;

  CORBA::ULong my_request_id;

  ACE_Thread_Mutex lock_;

  u_char buffer [CDR::DEFAULT_BUFSIZE];
  CDR stream;

  TAO_Client_Connection_Handler* handler_;
  // The handler for the client's connection.
#if 0
  autorelease <client_endpoint> endpoint;
#endif /* 0 */
};

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

// deprecated
typedef TAO_GIOP_LocateStatusType
(*TAO_GIOP_ForwardFunc) (TAO_opaque &,
                         CORBA::Object_ptr &,
                         void *);

// deprecated
typedef void (*TAO_GIOP_RequestHandler) (TAO_GIOP_RequestHeader &,
                                         CDR &,
                                         CDR *,
                                         void *,
                                         CORBA::Environment &);

class TAO_GIOP
// Only put static methods within this scope
{
public:
  // = Close a connection, first sending GIOP::CloseConnection
  static void close_connection (ACE_SOCK_Stream &fd, void *ctx);

  static CORBA::Boolean start_message (TAO_GIOP_MsgType t, CDR &msg);
  // Build the header for a message of type <t> into stream <msg>.
  
  static int incoming_message (ACE_SOCK_Stream &peer,
                               TAO_GIOP_ForwardFunc check_forward,
                               TAO_GIOP_RequestHandler handle_request,
                               void *context,
                               CORBA::Environment &env);

  static CORBA::Boolean send_message (CDR &stream, 
                                      ACE_SOCK_Stream &peer);

  // = Reads message, returns message type from header.

  static TAO_GIOP_MsgType read_message (ACE_SOCK_Stream &peer, 
                                        CDR &msg, 
                                        CORBA::Environment &env);

  
  static void make_error (CDR &msg, ...);
  // Construct a message containing an error so that it can be sent as
  // a response to a request.
};

#endif /* TAO_GIOP_H */
