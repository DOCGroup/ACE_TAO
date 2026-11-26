/** @file   Mem_Map_Stream.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */

#ifndef _ACEXML_MEM_MAP_STREAM_H
#define _ACEXML_MEM_MAP_STREAM_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Stream.h"
#include "ace/Mem_Map.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ACEXML/common/XML_Types.h"
#include <memory>

using Svc_Handler = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
using Connector = ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;

/**
 * @class ACEXML_Mem_Map_Stream
 *
 * @brief Provides a memory-mapped stream abstraction to simplify parsing
 * of tokens.
 *
 * This class makes it possible to treat an connection as a stream of
 * bytes, similar to the C library stdio streams. The contents of the
 * connection are buffered incrementally in a memory-mapped file. This
 * class maintains pointers to two positions in the stream:
 *
 *   1. The receive position, which keeps track of the beginning of a
 *      token that is in the stream.
 *
 *   2. The read position, which moves along character-by-character
 *      until the end of the token is reached.
 *
 * Once a token has been located, it can be extracted from the stream by
 * calling @c recv(). The length of the token, returned by @c recv_len(), is
 * the number of bytes between the read and receive positions. Once the token
 * has been extracted, the @c seek() method can update both positions.
 */
class ACEXML_Export ACEXML_Mem_Map_Stream
{
public:
  ACEXML_Mem_Map_Stream ();

  /// Initialize this object.
  virtual int open (Connector *connector,
                    ACE_INET_Addr const &);

  virtual ~ACEXML_Mem_Map_Stream ();

  /// Returns the underlying @c ACE_SOCK_Stream.
  ACE_SOCK_Stream &stream ();

  /**
   * Send @a size bytes from @a buf to the connected peer. This is a
   * completely unbuffered call.
   */
  virtual ssize_t send_n (void const *buf,
                          size_t size,
                          ACE_Time_Value *tv = nullptr);

  /**
   * Return the next character in the stream and advance the read position.
   * Returns EOF when the read position reaches the end of the HTTP stream.
   */
  virtual int get_char ();

  /**
   *  Returns a pointer to an array of at most @a len characters starting at
   *  the receive position. If the requested range extends past EOF, @a len
   *  is set to the number of characters between the receive position and
   *  EOF, and both positions are advanced by that amount. Returns 0 if the
   *  receive position is at EOF.
   */
  virtual char const *recv (size_t &len);

  /**
   *  Returns a pointer to an array of characters starting at the receive
   *  position.
   */
  virtual char const *recv () const;

  /**
   *  Returns the number of bytes between the read and receive positions.
   */
  virtual size_t recv_len () const;

  /**
   *  Returns the no. of bytes available in the stream.
   */
  virtual size_t available () const;

  /**
   *  Resets the read and receive positions to the beginning of the stream.
   *  This works since all the data has been cached in the memory-mapped
   *  backing store.
   */
  virtual void rewind ();

  /**
   *  Returns the character at @a offset from the read position without
   *  advancing that position. Automatically extends the backing store if
   *  necessary. Returns EOF if @a offset is past the
   *  end of the stream.
   */
  virtual int peek_char (size_t offset);

  /**
   *  Return a pointer to an array of @a size characters starting at
   *  @a offset from the read position without advancing that position.
   *  Automatically extends the backing store if necessary. Returns 0 if
   *  @a offset or @c offset + size is past the
   *  end of the stream.
   */
  virtual char const *peek_str (size_t offset, size_t size);

  /**
   *  Sets the read and receive positions as follows:
   *    o  If @a whence is @c SEEK_SET, the positions are set to @a offset
   *       bytes from the start of the stream.
   *    o  If @a whence is @c SEEK_CUR, the positions are set to the
   *       current read position plus @a offset.
   *    o  If @a whence is @c SEEK_END, the positions are set to the size
   *       of the stream plus @a offset.
   *
   *  If @a offset is greater than EOF, both positions are set to EOF.
   *  @note special return value is returned to indicate this condition.
   */
  virtual ACE_OFF_T seek (ACE_OFF_T offset, int whence = SEEK_CUR);

  /// Returns 1 if we're at the end of the HTTP stream, else 0.
  virtual int eof () const;

  /// Returns the underlying service handler.
  Svc_Handler *svc_handler ();

private:
  /**
   *  Grow the file by reading another chunk from the HTTP socket and
   *  extend the mapping to cover this chunk.  Returns -1 on failure or
   *  EOF, else 0.
   */
  int grow_file_and_remap ();

  /**
   *  Connection to peer. The granularity is at the Svc_Handler level.
   *  The Svc_Handler has an SOCK_Stream.
   */
  std::unique_ptr<Svc_Handler> svc_handler_ {};

  /// Memory-mapped file that we're iterating over.
  ACE_Mem_Map mem_map_;

  /// Pointer to the address where the next @c recv() call will start.
  char *recv_pos_;

  /**
   *  Pointer to the address where the next @c get_char() call will
   *  start.
   */
  char *get_pos_;

  /// Address at the end of the file mapping.
  char *end_of_mapping_plus1_;
};

#include /**/ "ace/post.h"
#endif /* _ACEXML_MEM_MAP_STREAM_H */
