// This may look like C, but it's really -*- C++ -*-

//======================================================================
/*
 *  @file Codeset_Manager_i.h
 *
 *  $Id$
 *
 *  Interface for the TAO CodeSet Manager.
 *
 *  @author  Phil Mesnier
 */
//======================================================================

#ifndef TAO_CODESET_MANAGER_I_H
#define TAO_CODESET_MANAGER_I_H

#include /**/ "ace/pre.h"

#include "tao/CONV_FRAMEC.h"
#include "tao/Codeset_Manager.h"
#include "tao/Codeset/codeset_export.h"
#include "tao/Codeset/Codeset_Descriptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Unbounded_Set.h"
#include "ace/Dynamic_Service_Dependency.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Profile;
class TAO_Transport;
class TAO_Operation_Details;

class TAO_ServerRequest;
class TAO_Tagged_Components;
class TAO_Codeset_Descriptor;

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

class TAO_Codeset_Export TAO_Codeset_Manager_i :
  public TAO_Codeset_Manager
{

public:
  /// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet
  /// No. 1
  static CONV_FRAME::CodeSetId default_char_codeset;
  /// NCS for wchar is defaulted to 0 (not used), but people wishing
  /// to provide a non-compliant default wchar codeset may do so.
  static CONV_FRAME::CodeSetId default_wchar_codeset;

  TAO_Codeset_Manager_i (void);
  ~TAO_Codeset_Manager_i (void);

  /// Called by an object of TAO_Acceptor to set NCS and CCS values
  /// for Char/Wchar in to the Object Reference.
  void set_codeset (TAO_Tagged_Components& ) const;
  ///
  /// Called from an object of "TAO_GIOP_Invocation" to set TCS on the
  /// Transport
  void set_tcs (TAO_Profile &theProfile, TAO_Transport &);

  /// Called from an Object of TAO_Messaging for every request at
  /// server side to process service context and set TCS for
  /// Char/WChar
  void process_service_context (TAO_ServerRequest &);

  /// Called by a client object to generate service context
  /// at this time Transport has the TCS for Char and WChar
  void generate_service_context (TAO_Operation_Details&, TAO_Transport & );

  /// Called by the resource factory to signify the end of
  /// initialization.  This will traverse the list of named codeset
  /// translator factories and add any of those that have a native
  /// codeset id matching the manager's native codeset id.
  void open(void);

  virtual TAO_Codeset_Descriptor_Base *char_codeset_descriptor (void);
  virtual TAO_Codeset_Descriptor_Base *wchar_codeset_descriptor (void);

private:
  // Compute the TCS for Char/WChar asper the CORBA Specification
  CONV_FRAME::CodeSetId computeTCS (CONV_FRAME::CodeSetComponent &,
                                    CONV_FRAME::CodeSetComponent &);

  // Find CodesetId in the codeset component
  int isElementOf (CONV_FRAME::CodeSetId,
                   CONV_FRAME::CodeSetComponent & );

  // find the intersection of CodesetIds between Client and Server CCS
  CONV_FRAME::CodeSetId intersectionOf (CONV_FRAME::CodeSetComponent &,
                                        CONV_FRAME::CodeSetComponent &);

  // determine compatibility between two codesets via the codeset
  // registry
  int isCompatible (CONV_FRAME::CodeSetId,  CONV_FRAME::CodeSetId);

  // traverse the list of codeset factories, populating the list of
  // conversion codeset values with the translated codeset id from
  // each factory that has a matching native codeset. Those factories
  // that do not have a matching codeset are not retained in the list.
  int init_ccs (TAO_Codeset_Descriptor&,
                CONV_FRAME::CodeSetComponent&);

  // get the translator between our ncs_c and the supplied tcs_c
  TAO_Codeset_Translator_Base * get_char_trans (CONV_FRAME::CodeSetId);

  // get the translator between our ncs_w and the supplied tcs_w
  TAO_Codeset_Translator_Base * get_wchar_trans (CONV_FRAME::CodeSetId);

  TAO_Codeset_Translator_Base * get_translator_i (TAO_Codeset_Descriptor&,
                                                  CONV_FRAME::CodeSetId);

  void get_ncs (CONV_FRAME::CodeSetId &ncsc, CONV_FRAME::CodeSetId& ncsw);

  // The CodeSetComponentInfo struct contains all of the information
  // regarding the code sets this application recognizes. This is
  // where the native code set for both char and wchar are stored.
  CONV_FRAME::CodeSetComponentInfo codeset_info_;

  TAO_Codeset_Descriptor char_descriptor_;
  TAO_Codeset_Descriptor wchar_descriptor_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_CODESET_MANAGER_I_H */
