/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ACE
// 
// = FILENAME
//    ACE_Name_Request_Reply.h
//
// = DESCRIPTION 
//     Define the format used to exchange messages between the 
//     ACE_Name Server and its clients.
//
// = AUTHOR
//    Gerhard Lenzer, Douglas C. Schmidt, and Prashant Jain
// 
// ============================================================================

#if !defined (ACE_NAME_REQUEST_REPLY_H)
#define ACE_NAME_REQUEST_REPLY_H

#include "ace/Time_Value.h"
#include "ace/SString.h"

class ACE_Export ACE_Name_Request
  // = TITLE
  //   Message format for delivering requests to the ACE_Name Server.
  //
  // = DESCRIPTION
  //   This class is implemented to minimize data copying.  
  //   In particular, all marshaling is done in situ...
{
public:
  enum Constants
  {
    /* Request message types. */
    BIND = 01, 
    REBIND = 02, 
    RESOLVE = 03, 
    UNBIND  = 04, 
    LIST_NAMES = 05,
    LIST_VALUES = 015,
    LIST_TYPES = 025,
    LIST_NAME_ENTRIES = 06,
    LIST_VALUE_ENTRIES = 016,
    LIST_TYPE_ENTRIES = 026,
    MAX_ENUM = 11, 
    MAX_LIST = 3,

    // Mask for bitwise operation used for table lookup
    OP_TABLE_MASK = 07,     // Mask for lookup of operation
    LIST_OP_MASK = 030,     // Mask for lookup of list_operation

    // Class-specific constant values.
    MAX_NAME_LENGTH = MAXPATHLEN + 1
  };

  ACE_Name_Request (void);
  // Default constructor.

  ACE_Name_Request (ACE_UINT32 msg_type, // Type of request.
		    const ACE_USHORT16 name[], //
		    const size_t name_length,
		    const ACE_USHORT16 value[],
		    const size_t value_length,
		    const char type[],
		    const size_t type_length,
		    ACE_Time_Value *timeout = 0); // Max time willing to wait for request.
  // Create a <ACE_Name_Request> message.  

  void init (void);
  // Initialize length_ in order to ensure correct byte ordering
  // before a request is sent.

  // = Set/get the length of the encoded/decoded message.
  ACE_UINT32 length (void) const;
  void length (ACE_UINT32);

  // = Set/get the type of the message.
  ACE_UINT32 msg_type (void) const;
  void msg_type (ACE_UINT32);

  // = Set/get the blocking semantics.
  ACE_UINT32 block_forever (void) const;
  void block_forever (ACE_UINT32);

  // = Set/get the timeout.
  ACE_Time_Value timeout (void) const;
  void timeout (const ACE_Time_Value timeout);

  // = Set/get the name 
  const ACE_USHORT16 *name (void) const;
  void name (const ACE_USHORT16 *);

  // = Set/get the value
  const ACE_USHORT16 *value (void) const;
  void value (const ACE_USHORT16 *);

  // = Set/get the type
  const char *type (void) const;
  void type (const char *);

  // = Set/get the len of name
  ACE_UINT32 name_len (void) const;
  void name_len (ACE_UINT32);

  // = Set/get the len of value
  ACE_UINT32 value_len (void) const;
  void value_len (ACE_UINT32);

  // = Set/get the len of type
  ACE_UINT32 type_len (void) const;
  void type_len (ACE_UINT32);

  int encode (void *&);
  // Encode the message before transmission.

  int decode (void);
  // Decode message after reception.

  void dump (void) const;
  // Print out the values of the message for debugging purposes.

private:
  // = The 5 fields in the <Transfer> struct are transmitted to the server.  
  // The remaining 2 fields are not tranferred -- they are used only on
  // the server-side to simplify lookups.

  struct Transfer
  {
    ACE_UINT32 length_;
    // Length of entire request.

    ACE_UINT32 msg_type_;
    // Type of the request (i.e., <BIND>, <REBIND>, <RESOLVE>, and <UNBIND>).

    ACE_UINT32 block_forever_;
    // Indicates if we should block forever.  If 0, then <secTimeout_> 
    // and <usecTimeout_> indicates how long we should wait.

    ACE_UINT32 sec_timeout_;
    // Max seconds willing to wait for name if not blocking forever.

    ACE_UINT32 usec_timeout_;
    // Max micro seconds to wait for name if not blocking forever.

    ACE_UINT32 name_len_;
    // Len of name in bytes

    ACE_UINT32 value_len_;
    // Len of value in bytes

    ACE_UINT32 type_len_;
    // Len of type in bytes

    ACE_USHORT16 data_[MAX_NAME_LENGTH + MAXPATHLEN + MAXPATHLEN + 2];
    // The data portion contains the <name_>
    // followed by the <value_>
    // followed by the <type_>.
  };

  Transfer transfer_;
  // Transfer buffer.

  ACE_USHORT16 *name_;
  // Pointer to the beginning of the name in this->data_.

  ACE_USHORT16 *value_;
  // Pointer to the beginning of the value in this->data_;

  char *type_;
  // Pointer to the beginning of the type in this->data_;
};

class ACE_Export ACE_Name_Reply
  // = TITLE
  //     Message format for delivering replies from the ACE_Name Server.
  //
  // = DESCRIPTION
  //   This class is implemented to minimize data copying.  
  //   In particular, all marshaling is done in situ...
{
public:
  enum Constants
  {
    /* Reply message types. */
    SUCCESS = 1, // Reply for successful operation.
    FAILURE = 2, // Reply for failed operation.

    /* Class-specific constant values. */
    MAX_NAME_LENGTH = MAXPATHLEN + 1
  };

  ACE_Name_Reply (void);
  // Default constructor.

  ACE_Name_Reply (ACE_UINT32 type, ACE_UINT32 err); // Type of reply.
  // Create a <ACE_Name_Reply> message.  

  void init (void);
  // Initialize length_ in order to ensure correct byte ordering
  // before a reply is sent.

  // = Set/get the length of the encoded/decoded message.
  ACE_UINT32 length (void) const;
  void length (ACE_UINT32);

  // = Set/get the type of the message.
  ACE_UINT32 msg_type (void) const;
  void msg_type (ACE_UINT32);

  // = Set/get the status of the reply (0 == success, -1 == failure).
  ACE_UINT32 status (void) const;
  void status (ACE_UINT32);

  // = Set/get the errno of a failed reply.
  ACE_UINT32 errnum (void) const;
  void errnum (ACE_UINT32);

  int encode (void *&);
  // Encode the message before transfer.

  int decode (void);
  // Decode a message after reception.

  void dump (void) const;
  // Print out the values of the message for debugging purposes.

private:
  // = The 3 fields in the <Transfer> struct are transmitted to the server.

  struct Transfer
  {
    ACE_UINT32 length_;
    // Length of entire reply.

    ACE_UINT32 type_;
    // Type of the reply (i.e., <SUCCESS> or <FAILURE>).

    ACE_UINT32 errno_;
    // Indicates why error occurred if <this->type_> == <FAILURE>.
    // Typical reasons include:
    //   <ETIME> (if the client timed out after waiting for the name).
  };

  Transfer transfer_;
  // Transfer buffer.
};

#endif /* ACE_NAME_REQUEST_REPLY_H */
