// -*- C++ -*-

// ===================================================================
/**
 *  @file   GIOP_Message_Generator_Parser_10.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_GENERATOR_PARSER_10_H
#define TAO_GIOP_MESSAGE_GENERATOR_PARSER_10_H

#include /**/ "ace/pre.h"

#include "tao/GIOP_Message_Generator_Parser.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_GIOP_Message_Generator_Parser_10
 *
 * @brief Implementation for GIOP v1.0
 *
 */

class TAO_GIOP_Message_Generator_Parser_10:
  public TAO_GIOP_Message_Generator_Parser
{
public:

  /// Write the request header in to @a msg
  virtual bool write_request_header (
      const TAO_Operation_Details &opdetails,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg);

  /// Write the LocateRequest header
  virtual bool write_locate_request_header (
      CORBA::ULong request_id,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg);

  /// Write the reply header in to @a output
  virtual bool write_reply_header (
      TAO_OutputCDR &output,
      TAO_Pluggable_Reply_Params_Base &reply);

  /// Writes the locate _reply message in to the @a output
  virtual bool write_locate_reply_mesg (
      TAO_OutputCDR &output,
      CORBA::ULong request_id,
      TAO_GIOP_Locate_Status_Msg &status);

  virtual bool write_fragment_header (TAO_OutputCDR & cdr,
                                      CORBA::ULong request_id);

  /// Parse the Request Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_request_header (TAO_ServerRequest &);

  /// Parse the LocateRequest Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_locate_header (
      TAO_GIOP_Locate_Request_Header &);

  /// Parse the reply message from the server
  virtual int parse_reply (TAO_InputCDR &input,
                           TAO_Pluggable_Reply_Params &params);

    /// Parse the reply message from the server
  virtual int parse_locate_reply (TAO_InputCDR &input,
                                  TAO_Pluggable_Reply_Params &params);

  /// Our versions
  virtual CORBA::Octet major_version (void) const;
  virtual CORBA::Octet minor_version (void) const;

  /// The header length of a fragment
  virtual size_t fragment_header_length (void) const;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_GIOP_MESSAGE_GENERATOR_PARSER_10_H*/
