/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps/Web
//
// = FILENAME
//    Mem_Map_Stream.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _MEM_MAP_STREAM_H
#define _MEM_MAP_STREAM_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE 
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Mem_Map.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Strategies_T.h"
#include "ace/Synch_T.h"

class Mem_Map_Stream
{
  // = TITLE
  //   Provides a memory-mapped stream abstraction to simplify parsing
  //   of tokens.  
  //
  // = DESCRIPTION
  //   This class makes it possible to treat an connection as a stream
  //   of bytes, similar to the C library stdio streams.  The contents
  //   of the connection are buffered incrementally in a memory-mapped
  //   file.  This class maintains pointers to two positions in the
  //   stream:
  //   
  //   1. The <recv> position, which keeps track of the beginning of a
  //      token that is in the stream.
  //  
  //   2. The <get> position, which moves along character-by-character
  //      until the end of the token is reached.
  // 
  //   Once a token has been located, it can be extracted from the
  //   stream by calling the <recv>.  The length of the token, i.e.,
  //   the <recv_len>, is the length in bytes between the <get>
  //   position and the <recv> position.  Once the token has been
  //   extracted, the <recv> and <get> positions can be updated by the
  //   <seek> method.

public:
  typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> Svc_Handler;
  
  typedef ACE_Strategy_Connector<Svc_Handler,
                               ACE_SOCK_CONNECTOR>
        STRAT_CONNECTOR;

  // Mem_Map_Stream (void);
  // constructor added:KIRTHIKA
  virtual int open (STRAT_CONNECTOR *connector, 
                    const ACE_INET_Addr &);
  // Initialize this object.

  virtual ~Mem_Map_Stream (void);
  // Destructor.

  // = Accessor.
  ACE_SOCK_Stream &stream (void);
  // Returns the underlying <ACE_SOCK_Stream>.

  // = I/O methods.

  virtual ssize_t send_n (const void *buf,
                          size_t size,
                          ACE_Time_Value *tv = 0);
  // Send <size> bytes in <buf> to the connected peer.  This is a
  // completely unbuffered call.

  virtual int get_char (void);
  // Return the next character in the stream and advance the <get>
  // position.  Returns EOF when the <get> position reaches the end of
  // the HTTP stream.
  
  virtual const char *recv (size_t &len);
  // Returns a pointer to array of at most <len> characters starting
  // at the <recv> position.  If the <recv> position + <len> extends
  // past the EOF then <len> is set to the number of characters
  // between the <recv> position and the EOF and both the <get> and
  // <recv> positions are advanced by <len>.  Returns 0 if the <recv>
  // position is at the EOF.

  virtual const char *recv (void) const;
  // Returns a pointer to array of characters starting at the <recv>
  // position.

  virtual size_t recv_len (void) const;
  // Returns the length in bytes between the <get> position and the
  // <recv> position.

  virtual int rewind (void);
  // Resets the <get> and <recv> positions to the beginning of the
  // stream.  This works since all the data has been cached in the
  // memory-mapped backing store.

  virtual int peek_char (size_t offset);
  // Returns the nth character <offset> from the <get> position in the
  // stream without advancing the <get> position.  Automatically
  // extends the backing store if necessary.  Returns EOF if <offset>
  // is past the end of the stream.

  virtual const char *peek_str (size_t offset, size_t size);
  // Return a pointer to an array of <size> characters starting at
  // <offset> characters from the <get> position in the stream without
  // advancing the <get> position.  Automatically extends the backing
  // store if necessary.  Returns 0 if <offset> or <offset + size> is
  // past the end of the stream.

  virtual off_t seek (off_t offset, int whence = SEEK_CUR);
  // Sets the <get> and <recv> positions as follows:
  //    o If <whence> is <SEEK_SET>, the positions are set to <offset>
  //      bytes from the start of the stream.
  // 
  //    o  If <whence> is <SEEK_CUR>, the positions are set to the
  //       current <get> position plus <offset>.
  //
  //    o  If <whence> is <SEEK_END>, the positions are set to the size
  //       of the stream plus <offset>.

  virtual int eof (void) const;
  // Returns 1 if we're at the end of the HTTP stream, else 0.

  
  /*
  typedef ACE_NOOP_Creation_Strategy<Svc_Handler>
  NULL_CREATION_STRATEGY;
  typedef ACE_NOOP_Concurrency_Strategy<Svc_Handler>
  NULL_ACTIVATION_STRATEGY;
  typedef ACE_Cached_Connect_Strategy<Svc_Handler,
                                    ACE_SOCK_CONNECTOR,
                                    ACE_SYNCH_NULL_MUTEX>
                                    CACHED_CONNECT_STRATEGY;*/
  
  Svc_Handler *svc_handler (void);

private:
  int grow_file_and_remap (void);
  // Grow the file by reading another chunk from the HTTP socket and
  // extend the mapping to cover this chunk.  Returns -1 on failure or
  // EOF, else 0.

  //ACE_SOCK_Stream stream_;
  
  Svc_Handler *svc_handler_;
  // Connection to peer. The granularity is at the Svc_Handler level.
  // The Svc_Handler has an SOCK_Stream.
  /*
  NULL_CREATION_STRATEGY creation_strategy_;
  NULL_ACTIVATION_STRATEGY activation_strategy_;
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy_;

  STRAT_CONNECTOR *strat_connector_;  */
 
  ACE_Mem_Map mem_map_;
  // Memory-mapped file that we're iterating over.

  char *recv_pos_;
  // Pointer to the address where the next <recv> method will start.

  char *get_pos_;
  // Pointer to the address where the next <get_char> method will
  // start.

  char *end_of_mapping_plus1_;
  // Address at the end of the file mapping.

};

#endif /* _MEM_MAP_STREAM_H */
 
