/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Token_Request_Reply.h
//
// = DESCRIPTION
//     Define the format used to exchange messages between the
//     ACE_Token Server and its clients.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//    Tim Harrison (harrison@cs.wustl.edu)
//
// ============================================================================

#ifndef ACE_TOKEN_REQUEST_REPLY_H
#define ACE_TOKEN_REQUEST_REPLY_H

#include "ace/Local_Tokens.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"

class ACE_Export ACE_Token_Request
{
  // = TITLE
  //   Message format for delivering requests to the ACE_Token Server.
  //
  // = DESCRIPTION
  //   This class is implemented to minimize data copying.
  //   In particular, all marshaling is done in situ...
public:
  enum OPERATION
  {
    // Operation types.
    ACQUIRE, // Acquire the token.
    RELEASE, // Release the token.
    RENEW,   // Renew the token.
    REMOVE,  // Remove the token.
    TRY_ACQUIRE
  };

  ACE_Token_Request (void);
  // Default constructor.

  ACE_Token_Request (int token_type,
                     int proxy_type,
                     ACE_UINT32 operation,
                     const char token_name[],
                     const char client_id[],
                     const ACE_Synch_Options &options);
  // token_type - MUTEX, RWLOCK
  // proxy_type - MUTEX, RLOCK, WLOCK (acquires mean different things)
  // operation - method
  // token_name
  // client_id
  // options - we check USE_TIMEOUT and use the arg.

  // = Set/get the length of the encoded/decoded message.
  ACE_UINT32 length (void) const;
  void length (ACE_UINT32);

  // = Set/get the type of proxy
  int proxy_type (void) const;
  void proxy_type (int proxy_type);

  // = Set/get the type of token
  int token_type (void) const;
  void token_type (int token_type);

  // = Set/get the type of the operation.
  ACE_UINT32 operation_type (void) const;
  void operation_type (ACE_UINT32);

  // = Set/get the requeue position.  These should be used when renew
  // is the operation type.
  ACE_UINT32 requeue_position (void) const;
  void requeue_position (ACE_UINT32);

  // = Set/get notify.  These should be used when acquire is the operation type.
  ACE_UINT32 notify (void) const;
  void notify (ACE_UINT32);

  // = Set/get the timeout.
  ACE_Synch_Options &options (void) const;
  void options (const ACE_Synch_Options &options);

  // = Set/get the name of the token and the client id.  The set
  // method is combined to make it easier on us.  We're copying the
  // names as a contiguous buffer.
  char *token_name (void) const;
  char *client_id (void) const;
  void token_name (const char *token_name, const char* client_id);

  int encode (void *&);
  // Encode the message before transmission.

  int decode (void);
  // Decode message after reception.  This must be called to set the
  // internal options.

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

    ACE_UINT32 token_type_;
    // Type of the request (i.e., MUTEX, RLOCK, WLOCK...

    ACE_UINT32 proxy_type_;
    // Type of the request (i.e., MUTEX, RLOCK, WLOCK...

    ACE_UINT32 operation_type_;
    // Type of the request (i.e., <ACQUIRE>, <RELEASE>, <RENEW>, and <REMOVE>).

    ACE_UINT32 requeue_position_;
    // this only makes sense when operation type is renew

    ACE_UINT32 notify_;
    // this only makes sense when operation type is renew

    // = ACE_Synch_Options stuff

    ACE_UINT32 use_timeout_;
    // Indicates if we should block forever.  If 1, then <secTimeout_>
    // and <usecTimeout_> indicates how long we should wait.  If 0,
    // then we block forever.

    ACE_UINT32 sec_;
    // Max seconds willing to wait for token if not blocking forever.

    ACE_UINT32 usec_;
    // Max micro seconds to wait for token if not blocking forever.

    ACE_UINT32 arg_;
    // value returned in Token_Reply::arg ();

    char data_[ACE_MAXTOKENNAMELEN + ACE_MAXCLIENTIDLEN + 1];
    // The data portion contains the <tokenName_> followed by a ':'
    // followed by the <clientId_>.
  } transfer_;

  char *token_name_;
  // Pointer to the beginning of the token name in this->data_.

  char *client_id_;
  // Pointer to the beginning of the client id in this->data_;

  ACE_Synch_Options options_;
  // Holds arg, sec, usec, etc.
};

class ACE_Export ACE_Token_Reply
{
  // = TITLE
  //     Message format for delivering replies from the ACE_Token Server.
  //
  // = DESCRIPTION
  //   This class is implemented to minimize data copying.
  //   In particular, all marshaling is done in situ...
public:
  ACE_Token_Reply (void);
  // Default constructor.

  // = Set/get the length of the encoded/decoded message.
  ACE_UINT32 length (void) const;
  void length (ACE_UINT32);

  // = Set/get the errno of a reply.
  ACE_UINT32 errnum (void) const;
  void errnum (ACE_UINT32);

  // = Set/get the arg of a reply.
  ACE_UINT32 arg (void) const;
  void arg (ACE_UINT32);

  int encode (void *&);
  // Encode the message before transfer.

  int decode (void);
  // Decode a message after reception.

  void dump (void) const;
  // Print out the values of the message for debugging purposes.

private:
  // = The 2 fields in the <Transfer> struct are transmitted to the server.

  struct Transfer
  {
    ACE_UINT32 length_;
    // Length of entire reply.

    ACE_UINT32 errno_;
    // Indicates why error occurred if <this->type_> == <FAILURE>.
    // Typical reasons include:
    //   <EWOULDBLOCK> (if client requested a non-blocking check for the token).
    //   <ETIME> (if the client timed out after waiting for the token).
    //   <ENOLCK> (if the token lock was removed out from underneath a waiter).
    //   <EACCES> (attempt to renew a token that isn't owned by the client).

    ACE_UINT32 arg_;
    // magic cookie

  } transfer_;
};

#if defined (__ACE_INLINE__)
#include "ace/Token_Request_Reply.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_TOKEN_REQUEST_REPLY_H */
