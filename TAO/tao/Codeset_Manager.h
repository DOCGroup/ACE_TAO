// -*- C++ -*-

//=============================================================================
/*
 *  @file Codeset_Manager.h
 *
 *  $Id$
 *
 *  Interface for the TAO CodeSet Manager.
 *
 *  @author  Mahesh Vedantam <mahesh@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CODESET_MANAGER_H
#define TAO_CODESET_MANAGER_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_WChar_Codeset_Translator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;
class TAO_Transport;
class TAO_Operation_Details;

class TAO_ServerRequest;
class TAO_Tagged_Components;
class TAO_Codeset_Translator_Base;
class TAO_Codeset_Descriptor_Base;


// ****************************************************************

/**
 * @class TAO_Codeset_Manager
 *
 * @brief The encapsulation of logic for codeset negotiation
 *
 * The Codeset Manager is owned by the ORB_Core, initialized through Resource
 * Factory configuration options. The codeset manager participates in profile
 * creation by servers and connection establishment by clients. The involvement
 * is necessary to supply a codeset component to the profile including for both
 * chars and wide chars the native code set and any conversion code sets for
 * which translators are available. The codeset manager is also responsible for
 * determining the transmission codesets based an the local and remote codeset
 * information. The transmission codesets are communicated via a service
 * context attached to the first request sent on the new connection.
 *
 */
class TAO_Export TAO_Codeset_Manager
{

public:

  /// Destructor.
  virtual ~TAO_Codeset_Manager (void);

  /// Called by an object of TAO_Acceptor to set NCS and CCS values for
  /// Char/Wchar in to the Object Reference.
  virtual void set_codeset (TAO_Tagged_Components& ) const = 0;

  /// Called from an object of "TAO_GIOP_Invocation" to set TCS on the
  /// Transport
  virtual void set_tcs (TAO_Profile &theProfile, TAO_Transport &) = 0;

  /// Called from an Object of TAO_Messaging for every request at server side
  /// to process service context and set TCS for Char/WChar
  virtual void process_service_context (TAO_ServerRequest &) = 0;

  /// Called by a client object to generate service context
  /// at this time Transport has the TCS for Char and WChar
  virtual void generate_service_context (TAO_Operation_Details&, TAO_Transport & ) = 0;

  virtual TAO_Codeset_Translator_Base * get_char_trans (CONV_FRAME::CodeSetId tcs) = 0;

  virtual TAO_Codeset_Translator_Base * get_wchar_trans (CONV_FRAME::CodeSetId tcs) = 0;

  virtual void open (void) = 0;

  virtual TAO_Codeset_Descriptor_Base *char_codeset_descriptor (void) = 0;
  virtual TAO_Codeset_Descriptor_Base *wchar_codeset_descriptor (void) = 0;

  virtual void get_ncs (CONV_FRAME::CodeSetId &ncsc,
                        CONV_FRAME::CodeSetId &ncsw) = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_MANAGER_H */
