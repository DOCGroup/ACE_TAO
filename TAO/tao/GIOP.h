// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Giop.h
//
// = DESCRIPTION
//     GIOP data structures and support routines
//
//     Note that some symbols in this module are part of the
//     "Internet" Inter-ORB Protocol (IIOP), not the General IOP.
//     Only addressing information and certain details of connection
//     usage are specific to IIOP; all other protocol details can be
//     reused by ORB protocols that are built atop connection
//     protocols other than TCP.
//
//     THREADING NOTE: Threads should never manipulate another
//     thread's invocations.  In this implementation, all data
//     structures used to represent invocations (and parts of them)
//     are owned by the thread which created them.  Multiple threads
//     may make of course concurrent invocations safely, since the
//     GIOP code is reentrant.
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.,
//     Chris Cleeland <cleeland@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_GIOP_H
#define TAO_GIOP_H

#include "tao/corbafwd.h"
#include "tao/Sequence.h"
#include "tao/CDR.h"

class TAO_Transport;
class TAO_ORB_Core;

// namespace TAO_IOP

// Assigned Protocol/Profile tag values.  ORB protcols may be uniquely
// identified by tags such as these.  This allows each ORB's own
// objref profiles to be interchanged using IORs.
//
// Email to tag-request@omg.org to allocate tags.
typedef CORBA::ULong TAO_IOP_Profile_ID;

enum
{
  TAO_IOP_TAG_INVALID = -1,
  TAO_IOP_TAG_INTERNET_IOP = 0,                // IIOP
  TAO_IOP_TAG_MULTIPLE_COMPONENTS = 1,         // DCE-CIOP
  TAO_IOP_TAG_UNIX_IOP = TAO_TAG_UIOP_PROFILE, // Read corbafwd.h

  // = This is a subset of the list of other profile tags.
  TAO_IOP_TAG_ONC_IOP = 0x4f4e4300     // ONC IOP
};

class TAO_IOP_Tagged_Profile
{
  // = TITLE
  //   One per protocol.
public:
  TAO_IOP_Profile_ID tag;
  TAO_opaque profile_data;
};

typedef TAO_Unbounded_Sequence<TAO_IOP_Tagged_Profile>
        TAO_IOP_Tagged_Profile_Sequence;

class TAO_IOP_IOR
{
  // = TITLE
  //   InteroperableObjectReference
  //
  // = DESCRIPTION
  //   ... a set of protocol-specific protocol profiles, plus a type
  //   ID.  Only one object is denoted by all of this information.
  //   It's OK to delete all profiles except the one for the single
  //   protocol actually being used.

public:
  char *type_id;
  TAO_IOP_Tagged_Profile_Sequence profiles;
};

// Some protocols can be factored into a set of optional components.
// Use of such components is defined by the protocol's specification.
//
// Email to tag-request@omg.org to allocate tags.

typedef CORBA::ULong TAO_IOP_ComponentId;

enum
{
  // = DESCRIPTION
  //   These are all defined by DCE-CIOP in OMG TC document 95-3-10.

  TAO_IOP_TAG_DCE_STRING_BINDING = 100,// string binding handle
  TAO_IOP_TAG_DCE_BINDING_NAME = 101, // CDS/GDS/... name
  TAO_IOP_TAG_DCE_NO_PIPES = 102, // no component data
  TAO_IOP_TAG_OBJECT_KEY = 10,  // opaque
  TAO_IOP_TAG_ENDPOINT_ID = 11, // uuid
  TAO_IOP_TAG_LOCATION_POLICY = 12 // octet/enum
};

class TAO_IOP_TaggedComponent
{
  // = DESCRIPTION
  //   One way to represent multicomponent profiles, e.g. as done by
  //   the DCE-CIOP protocol.  One of these gets encapsulated in
  //   Tagged_Profile::profile_data.  TAG_MULTIPLE_COMPONENTS may be
  //   used to represent protocol profiles structured in that way, but
  //   protocol-specific tags facilitate simpler scanning of IORs
  //   since you can be assured that each profile only has data used
  //   within a single ORB protocol.
public:

  TAO_IOP_ComponentId tag;
  TAO_opaque component_data;
};

// ****************************************************************

typedef TAO_Unbounded_Sequence<TAO_IOP_TaggedComponent>
        TAO_IOP_MultipleComponentProfile;

class TAO_Export TAO_GIOP_Version
{
  // = TITLE
  //   Major and Minor version number of the Inter-ORB Protocol.
public:
  CORBA::Octet major;
  // Major version number

  CORBA::Octet minor;
  // Minor version number

  TAO_GIOP_Version (const TAO_GIOP_Version &src);
  // Copy constructor

  TAO_GIOP_Version (CORBA::Octet maj = TAO_DEF_GIOP_MAJOR,
                    CORBA::Octet min = TAO_DEF_GIOP_MINOR);
  // Default constructor.

  ~TAO_GIOP_Version (void);
  // Destructor.

  void set_version (CORBA::Octet maj, CORBA::Octet min);
  // Explicitly set the major and minor version.

  TAO_GIOP_Version &operator= (const TAO_GIOP_Version &src);
  // Copy operator.

  int operator== (const TAO_GIOP_Version &src);
  int operator!= (const TAO_GIOP_Version &src);
  // Equality operator
};

// ****************************************************************

class TAO_Export TAO_GIOP_Message_State
{
  // = TITLE
  //   Represent the state of an incoming GIOP message.
  //
  // = DESCRIPTION
  //   As the ORB processes a GIOP messages it needs to keep track of
  //   how much of the message has been read, if there are any
  //   fragments following this message, the contents of the header,
  //   etc.
  //   Notice that it is not a direct mapping from the
  //   GIOP::MessageHeader IDL structure, for example, the message
  //   flags introduced in GIOP 1.1 are expanded in several fields.

public:
  TAO_GIOP_Message_State (TAO_ORB_Core* orb_core);
  // Constructor.
  // The parameters are used to initialize the InputCDR stream

  void reset (void);
  // Reset the message header state and prepare it to receive the next
  // event.
  // already read.
  // This method will reset the header to indicate that the last
  // message was procesno data (not
  // event the

  int header_received (void) const;
  // Has the header been received?

  int is_complete (void);
  // Check if the current message is complete, adjusting the fragments
  // if required...

  // = The GIOP header for the current fragment...
  CORBA::Octet magic [4];        // "GIOP"
  TAO_GIOP_Version giop_version; // 1.1 or 1.0
  CORBA::Octet byte_order;       // 0 = big, 1 = little
  CORBA::Octet more_fragments;   // For GIOP 1.1 (Requests and Replys)
  CORBA::Octet message_type;     // MsgType above
  CORBA::ULong message_size;     // in byte_order!

  CORBA::ULong current_offset;
  // How much of the payload has been received

  TAO_InputCDR cdr;
  // This is the InputCDR that will be used to decode the message.

  ACE_Message_Block* fragments_begin;
  ACE_Message_Block* fragments_end;
  // The fragments are collected in a chain of message blocks (using
  // the cont() field).  When the complete message is received the
  // chain is reassembled into <cdr>

  CORBA::Octet first_fragment_byte_order;
  // The byte order for the the first fragment
  // @@ The current implementation cannot handle fragments with
  //    different byte orders, this should not be a major problem
  //    because:
  //    1) It is unlikely that we are going to receive fragments.
  //    2) The spec *seems* to allow different byte_orders, but it is
  //       unlikely that any ORB will do that.
  //    3) Even if we allowed that at this layer the CDR classes are
  //       not prepared to handle that.

  TAO_GIOP_Version first_fragment_giop_version;
  // The GIOP version for the first fragment
  // @@ Same as above, all GIOP versions must match.

  CORBA::Octet first_fragment_message_type;
  // If the messages are chained this represents the message type for
  // the *complete* message (remember that the last message will be
  // fragment and the upper level needs to know if it is a request,
  // locate request or what).

private:
  int append_fragment (ACE_Message_Block* current);
  // Append <current> to the list of fragments
  // Also resets the state, because the current message was consumed.
};

// The GIOP message header size
#define TAO_GIOP_HEADER_LEN 12

// The offset the message_size field inside the GIOP HEADER
#define TAO_GIOP_VERSION_MAJOR_OFFSET 4
#define TAO_GIOP_VERSION_MINOR_OFFSET 5
#define TAO_GIOP_MESSAGE_FLAGS_OFFSET 6
#define TAO_GIOP_MESSAGE_TYPE_OFFSET 7
#define TAO_GIOP_MESSAGE_SIZE_OFFSET 8

// The IIOP Lite header length and the offset of the message size
// field in it.
#define TAO_GIOP_LITE_HEADER_LEN 5
#define TAO_GIOP_LITE_MESSAGE_SIZE_OFFSET 0
#define TAO_GIOP_LITE_MESSAGE_TYPE_OFFSET 4

// Support for Implicit ORB Service Context.
typedef CORBA::ULong TAO_GIOP_ServiceID;

enum
{
  // = TITLE
  //   Values for TAO_GIOP_ServiceID.

  TAO_GIOP_TransactionService = 0

  // More service IDs may be defined by OMG.
  // This is where our RIOP service ID will be defined...
};

// namespace TAO_GIOP

class TAO_GIOP_ServiceContext
{
  // = TITLE
  //   Sent for service context entries in the GIOP request header.
public:
  TAO_GIOP_ServiceID context_id;
  TAO_opaque context_data;
};

typedef TAO_Unbounded_Sequence<TAO_GIOP_ServiceContext>
        TAO_GIOP_ServiceContextList;

extern CORBA::TypeCode_ptr TC_ServiceContextList;

extern TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_GIOP_ServiceContext&);

extern TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_GIOP_ServiceContext&);

extern TAO_Export CORBA::Boolean
operator<< (TAO_OutputCDR&, const TAO_GIOP_ServiceContextList&);

extern TAO_Export CORBA::Boolean
operator>> (TAO_InputCDR&, TAO_GIOP_ServiceContextList&);

// = Reply headers

enum TAO_GIOP_ReplyStatusType
{
  TAO_GIOP_NO_EXCEPTION,
  // Request completed successfully

  TAO_GIOP_USER_EXCEPTION,
  // Request terminated with user exception

  TAO_GIOP_SYSTEM_EXCEPTION,
  // Request terminated with system exception

  TAO_GIOP_LOCATION_FORWARD
  // Reply is a location forward type
};

class TAO_GIOP_ReplyHeader
{
  // = TITLE
  //   This class embodies the header of a GIOP reply.
public:
  TAO_GIOP_ServiceContextList service_info;
  // Information

  CORBA::ULong request_id;
  // Unique identifier of the request for which this is a reply.

  TAO_GIOP_ReplyStatusType reply_status;
  // Status of the reply (see above enum).
};

class TAO_GIOP_CancelRequestHeader
{
  // = TITLE
  //   Cancellation -- applies both to Requests and LocateRequests.
public:
  CORBA::ULong request_id;
  // Unique identifier of the request being cancelled
};

class TAO_GIOP_LocateRequestHeader
{
  // = TITLE
  //   Location service support
public:
  TAO_GIOP_LocateRequestHeader (void);
  // Constructor

  void init (TAO_InputCDR &msg,
             CORBA_Environment &TAO_IN_ENV =
                 TAO_default_environment ());
  // Initialize the header from the values found in <msg>.

  CORBA::ULong request_id;
  TAO_opaque object_key;
};

enum TAO_GIOP_LocateStatusType
{
  TAO_GIOP_UNKNOWN_OBJECT,
  TAO_GIOP_OBJECT_HERE,
  TAO_GIOP_OBJECT_FORWARD
};

class TAO_GIOP_LocateReplyHeader
{
  // = TITLE
  //   Location reply.
public:
  CORBA::ULong request_id;
  TAO_GIOP_LocateStatusType locate_status;
};

class TAO_Export TAO_GIOP
{
  // = TITLE
  //   A namespace for GIOP-related operations.
  //
  // = DESCRIPTION
  //   Only put static methods within this scope.
  ACE_CLASS_IS_NAMESPACE (TAO_GIOP);

public:

  enum Message_Type
  {
    // = DESCRIPTION
    //   All GIOP messages include a header and message type.  Not
    //   really a message type, but needed to bring that information
    //   back somehow.

    // = GIOP message types.
    Request = 0,                // sent by client.
    Reply = 1,                  // by server.
    CancelRequest = 2,          // by client.
    LocateRequest = 3,          // by client.
    LocateReply = 4,            // by server.
    CloseConnection = 5,        // by server.
    MessageError = 6,           // by both.
    Fragment = 7                // by both.
  };

  static void close_connection (const TAO_GIOP_Version &version,
                                TAO_Transport *transport,
                                void *ctx);
  // Close a connection, first sending GIOP::CloseConnection.

  static CORBA::Boolean start_message (const TAO_GIOP_Version &version,
                                       TAO_GIOP::Message_Type t,
                                       TAO_OutputCDR &msg,
                                       TAO_ORB_Core* orb_core);
  // Build the header for a message of type <t> into stream <msg>.

  static CORBA::Boolean write_request_header (CORBA::ULong request_id,
                                              CORBA::Boolean is_roundtrip,
                                              const TAO_opaque& key,
                                              const char* opname,
                                              CORBA::Principal_ptr principal,
                                              TAO_OutputCDR &msg,
                                              TAO_ORB_Core *orb_core);
  // Write the GIOP request header.

  static CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                                     const TAO_opaque &key,
                                                     TAO_OutputCDR &msg);
  // Write the GIOP locate request header.

  static int send_message (TAO_Transport *transport,
                           TAO_OutputCDR &stream,
                           TAO_ORB_Core* orb_core);
  // Send message, returns TRUE if success, else FALSE.

  static void dump_msg (const char *label,
                        const u_char *ptr,
                        size_t len);
  // Print out a message header.

  static void send_error (const TAO_GIOP_Version &version,
                          TAO_Transport *transport);
  // Send an error message back to a caller.

  static ssize_t read_buffer (TAO_Transport *transport,
                              char *buf,
                              size_t len);
  // Loop on data read ... this is required since <recv> won't block
  // until the requested amount of data is available.

  static TAO_GIOP_ReplyStatusType convert_CORBA_to_GIOP_exception (CORBA::exception_type corba_type);
  // Convert the exception type from CORBA to GIOP

  static int read_header (TAO_Transport *transport,
                          TAO_ORB_Core *orb_core,
                          TAO_GIOP_Message_State &state,
                          CORBA::ULong &header_size,
                          TAO_InputCDR &input);
  static int handle_input (TAO_Transport *transport,
                           TAO_ORB_Core *orb_core,
                           TAO_GIOP_Message_State &state);

  static int parse_reply (TAO_Transport *transport,
                          TAO_ORB_Core *orb_core,
                          TAO_GIOP_Message_State& state,
                          TAO_GIOP_ServiceContextList& reply_ctx,
                          CORBA::ULong& request_id,
                          CORBA::ULong& reply_status);
  static void process_server_message (TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core,
                                      TAO_InputCDR &input,
                                      const TAO_GIOP_Message_State& state);

  static void process_server_request (TAO_Transport *transport,
                                      TAO_ORB_Core* orb_core,
                                      TAO_InputCDR &input,
                                      TAO_OutputCDR &output,
                                      const TAO_GIOP_Version& version);
  // A request was received on the server side.
  // <transport> is the source of the message (and thus where the
  // replies should be sent).
  // <orb_core> is the ORB that received the message
  // <input> contains the message
  // <output> can be used to store any responses
  // <request_id> and <response_required> are set as part of the
  // message processing.

  static void process_server_locate (TAO_Transport *transport,
                                     TAO_ORB_Core* orb_core,
                                     TAO_InputCDR &input,
                                     TAO_OutputCDR &output,
                                     const TAO_GIOP_Version& version);
  // A LocateRequest was received on the server side.
  // <transport> is the source of the message (and thus where the
  // replies should be sent).
  // <orb_core> is the ORB that received the message
  // <input> contains the message
  // <output> can be used to store any responses
  // <request_id> and <response_required> are set as part of the
  // message processing.

  static int send_reply_exception (const TAO_GIOP_Version &version,
                                   TAO_Transport *transport,
                                   TAO_ORB_Core* orb_core,
                                   CORBA::ULong request_id,
                                   CORBA::Exception *x);
  // We must send a LocateReply through <transport>, this request
  // resulted in some kind of exception.

private:
  static CORBA::Boolean start_message_std (const TAO_GIOP_Version &version,
                                           TAO_GIOP::Message_Type t,
                                           TAO_OutputCDR &msg);
  // Build the standard header for a message of type <t> into
  // stream <msg>.

  static CORBA::Boolean start_message_lite (const TAO_GIOP_Version &version,
                                            TAO_GIOP::Message_Type t,
                                            TAO_OutputCDR &msg);
  // Build the lightweight header for a message of type <t> into
  // stream <msg>.

  static CORBA::Boolean write_request_header_std (const TAO_GIOP_ServiceContextList& svc_ctx,
                                                  CORBA::ULong request_id,
                                                  CORBA::Boolean is_roundtrip,
                                                  const TAO_opaque& key,
                                                  const char* opname,
                                                  CORBA::Principal_ptr principal,
                                                  TAO_OutputCDR &msg);
  // Encode the standard header for the Request, assuming that the
  // GIOP header is already there.

  static CORBA::Boolean write_request_header_lite (const TAO_GIOP_ServiceContextList& svc_ctx,
                                                   CORBA::ULong request_id,
                                                   CORBA::Boolean is_roundtrip,
                                                   const TAO_opaque& key,
                                                   const char* opname,
                                                   CORBA::Principal_ptr principal,
                                                   TAO_OutputCDR &msg);
  // Encode the light weight header for the Request, assuming that the
  // GIOP header is already there.

  static int parse_header (TAO_ORB_Core *orb_core,
                           TAO_InputCDR &input,
                           TAO_GIOP_Message_State& state);
  // Parse the header, extracting all the relevant info.

  static int parse_header_std (TAO_InputCDR &input,
                               TAO_GIOP_Message_State& state);

  static int parse_header_lite (TAO_InputCDR &input,
                                TAO_GIOP_Message_State& state);
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_GIOP_H */
