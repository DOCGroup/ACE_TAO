// -*- C++ -*-

// ===================================================================
/**
 *  @file   GIOP_Message_Generator_Parser.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_GENERATOR_PARSER_H
#define TAO_GIOP_MESSAGE_GENERATOR_PARSER_H

#include /**/ "ace/pre.h"
#include "ace/Global_Macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/TAO_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_OutputCDR;
class TAO_InputCDR;
class TAO_Pluggable_Reply_Params;
class TAO_Pluggable_Reply_Params_Base;
class TAO_GIOP_Locate_Status_Msg;
class TAO_GIOP_Locate_Request_Header;
class TAO_ServerRequest;

/**
 * @class TAO_GIOP_Message_Generator_Parser
 *
 * @brief Base class for the GIOP Message generator and parser
 *
 * Version specific classes would inherit from this class. Strategy to
 * determine which version of the GIOP request have we received and
 * which version do we need to use to send messages.
 */

class TAO_GIOP_Message_Generator_Parser
{
public:

  virtual ~TAO_GIOP_Message_Generator_Parser (void);

  /// Write the request header in to @a msg
  virtual bool write_request_header (
      const TAO_Operation_Details &opdetails,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg) = 0;

  /// Write the LocateRequest header
  virtual bool write_locate_request_header (
      CORBA::ULong request_id,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg) = 0;

  /// Write the reply header in to @a output
  virtual bool write_reply_header (
      TAO_OutputCDR &output,
      TAO_Pluggable_Reply_Params_Base &reply) = 0;

  /// Writes the locate _reply message in to the @a output
  virtual bool write_locate_reply_mesg (
      TAO_OutputCDR &output,
      CORBA::ULong request_id,
      TAO_GIOP_Locate_Status_Msg &status) = 0;

  /// Write the GIOP fragment message header to the output CDR stream
  /// @a cdr.
  virtual bool write_fragment_header (TAO_OutputCDR & cdr,
                                      CORBA::ULong request_id) = 0;

  /// Parse the Request Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_request_header (TAO_ServerRequest &) = 0;

  /// Parse the Locate Request Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &) = 0;

  /// Parse the reply message
  virtual int parse_reply (TAO_InputCDR &input,
                           TAO_Pluggable_Reply_Params &params);

  /// Parse the locate reply message from the server
  virtual int parse_locate_reply (TAO_InputCDR &input,
                                  TAO_Pluggable_Reply_Params &params);

  /// Our versions
  virtual CORBA::Octet major_version (void) const = 0;
  virtual CORBA::Octet minor_version (void) const = 0;

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  virtual bool is_ready_for_bidirectional (void) const;

  /// The header length of a fragment
  virtual size_t fragment_header_length (void) const = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_GIOP_MESSAGE_GENERATOR_PARSER_H*/
