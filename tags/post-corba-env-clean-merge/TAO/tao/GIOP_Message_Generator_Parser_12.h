
// -*- C++ -*-
// ===================================================================
/**
 *  @file   GIOP_Message_Generator_Parser_12.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_GIOP_MESSAGE_GENERATOR_PARSER_12_H
#define TAO_GIOP_MESSAGE_GENERATOR_PARSER_12_H
#include "ace/pre.h"
#include "tao/GIOP_Message_Generator_Parser.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_GIOP_Message_State;
class TAO_Service_Context;
class TAO_Transport;

/**
 * @class TAO_GIOP_Message_Generator_Parser_12
 *
 * @brief Implementation for GIOP v1.2
 *
 */

class TAO_Export TAO_GIOP_Message_Generator_Parser_12:
  public TAO_GIOP_Message_Generator_Parser
{
public:

  /// Write the request header in to <msg>
  virtual int write_request_header (
      const TAO_Operation_Details &opdetails,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg
    );

  /// Write the LocateRequest header
  virtual int write_locate_request_header (
      CORBA::ULong request_id,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg
    );

  /// Write the reply header in to <output>
  virtual int write_reply_header (
      TAO_OutputCDR &output,
      TAO_Pluggable_Reply_Params_Base &reply
      TAO_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Writes the locate _reply message in to the <output>
  virtual int write_locate_reply_mesg (
      TAO_OutputCDR &output,
      CORBA::ULong request_id,
      TAO_GIOP_Locate_Status_Msg &status
    );

  /// Parse the Request Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_request_header (TAO_ServerRequest &);

  /// Parse the LocateRequest Header from the incoming stream. This will do a
  /// version specific parsing of the incoming Request header
  virtual int parse_locate_header (
      TAO_GIOP_Locate_Request_Header &
    );

  /// Parse the reply message
  virtual int parse_reply (TAO_InputCDR &input,
                           TAO_Pluggable_Reply_Params &params);

  /// Parse the locate reply message
  virtual int parse_locate_reply (TAO_InputCDR &input,
                                  TAO_Pluggable_Reply_Params &params);

  /// Our versions
  virtual CORBA::Octet major_version (void);
  virtual CORBA::Octet minor_version (void);

  /// Is the messaging object ready for processing BiDirectional
  /// request/response?
  virtual int is_ready_for_bidirectional (void);

private:

  /// Marshall the TargetSpecification

  /// This method may be required for other GIOP versiona coming out
  /// later than 1.2. We need to share this method
  int marshall_target_spec (TAO_Target_Specification &spec,
                            TAO_OutputCDR &msg);

  /// Check whether we have BiDirContext info available. If available
  /// delegate  the responsibility on to the TAO_Transport classes to
  /// initiate action.
  /// Note: At somepoint this may be needed for future versions of
  /// GIOP and we may have to share this
  int check_bidirectional_context (TAO_ServerRequest &);

  /// Process the BiDirContext info that we have received.
  int process_bidir_context (TAO_Service_Context &,
                             TAO_Transport *transport);
};


#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Generator_Parser_12.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_GENERATOR_PARSER_12_H*/
