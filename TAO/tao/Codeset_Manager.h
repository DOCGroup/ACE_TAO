// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/*
 *  @file Codeset_Manager.h
 *
 *  $Id$
 *
 *  Interface for the TAO CodeSet Manager.
 *
 *  @author  Mahesh Vedantam <mahesh#ociweb.com>
 */
//=============================================================================

#ifndef TAO_CODESET_MANAGER_H
#define TAO_CODESET_MANAGER_H
#include "ace/pre.h"

#include "tao/CONV_FRAMEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Profile;
class TAO_Transport;
class TAO_Operation_Details;

class TAO_ServerRequest;
class TAO_Tagged_Components;
class TAO_Codeset_Translator_Factory;
class TAO_Codeset_Manager;

// ****************************************************************

/**
 * @class TAO_Codeset_Item
 *
 * @brief A single element in the list of Codeset Translator Factories
 *
 * This class is used by the Codeset_Manager to store references to
 * individual codeset factories. Only those translators that match the
 * char or wchar native codeset will be kept in the list.
 *
 */

class TAO_Codeset_Item
{
public:
  friend class TAO_Codeset_Manager;
  /// creator method, the codeset name can only be set when the
  /// object is created.
  TAO_Codeset_Item (const char *name);

  /// destructor that deallocates the factory object if the
  /// CodeSet_Item retains ownership.
  ~TAO_Codeset_Item (void);

  /// return a reference to the character representation of the codeset
  /// factories name.
  const char *codeset_name (void);

  /// return a pointer to the codeset factory.
  TAO_Codeset_Translator_Factory *factory (void);

  /// set the factory pointer's value.
  void factory (TAO_Codeset_Translator_Factory *factory);

private:
  // Prohibited
  ACE_UNIMPLEMENTED_FUNC (TAO_Codeset_Item (const TAO_Codeset_Item&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Codeset_Item&))

private:
  /// factory name.
  char *name_;

  /// pointer to factory object.
  TAO_Codeset_Translator_Factory *factory_;
};

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
  /// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
  static CONV_FRAME::CodeSetId default_char_codeset;
  /// NCS for wchar is defaulted to 0 (not used), but people wishing to
  /// provide a non-compliant default wchar codeset may do so.
  static CONV_FRAME::CodeSetId default_wchar_codeset;

  TAO_Codeset_Manager ();
  ~TAO_Codeset_Manager();

  ///  Called by an object of TAO_Acceptor to set NCS and CCS values for
  ///  Char/Wchar in to the Object Reference.
  void set_codeset (TAO_Tagged_Components& ) const;
  ///
  /// Called from an object of "TAO_GIOP_Invocation" to set TCS on the
  /// Transport
  void set_tcs (TAO_Profile &theProfile, TAO_Transport &);

  /// Called from an Object of TAO_Messaging for every request at server side
  /// to process service context and set TCS for Char/WChar
  void process_service_context (TAO_ServerRequest &);

  /// Called by a client object to generate service context
  /// at this time Transport has the TCS for Char and WChar
  void generate_service_context (TAO_Operation_Details&, TAO_Transport & );


  /// Called by the resource factory to set the native char codeset id
  void set_ncs_c (CONV_FRAME::CodeSetId ncs);

  /// Called by the resource factory to set the native wchar codeset id. The
  /// maxbytes value is used to communicate the width of untranslated wide
  /// characters on the stream. This size may be smaller than the size of a
  /// wchar_t.
  void set_ncs_w (CONV_FRAME::CodeSetId ncs, int maxbytes = 0);

  /// Called by the resource factory to add a potential codeset translator
  /// for char data. The actual factory will be added to the list later, if its
  /// ncs matches that of the codeset manager.
  int add_char_translator (const char *name);

  /// Called by the resource factory to add a potential codeset translator
  /// for wchar data. The actual factory will be added to the list later, if
  /// its ncs matches that of the codeset manager.
  int add_wchar_translator (const char *name);

  /// Called by the resource factory to signify the end of initialization.
  /// This will traverse the list of named codeset translator factories and add
  /// any of those that have a native codeset id matching the manager's native
  /// codeset id.
  void configure_codeset_factories();

private:

// typedefs for containers containing the list of codesets
// factories for character and wide character.
typedef ACE_Unbounded_Set<TAO_Codeset_Item*>
        TAO_CodesetFactorySet;

// Iterators
typedef ACE_Unbounded_Set_Iterator<TAO_Codeset_Item*>
        TAO_CodesetFactorySetItor;



  // Compute the TCS for Char/WChar asper the CORBA Specification
  CONV_FRAME::CodeSetId computeTCS (CONV_FRAME::CodeSetComponent &,
                                    CONV_FRAME::CodeSetComponent &);

  // Find CodesetId in the codeset component
  int isElementOf (CONV_FRAME::CodeSetId,
                   CONV_FRAME::CodeSetComponent & );

  // find the intersection of CodesetIds between Client and Server CCS
  CONV_FRAME::CodeSetId intersectionOf (CONV_FRAME::CodeSetComponent &,
                                        CONV_FRAME::CodeSetComponent &);

  // determine compatibility between two codesets via the codeset registry
  int isCompatible (CONV_FRAME::CodeSetId,  CONV_FRAME::CodeSetId);

  // traverse the list of codeset factories, populating the list of conversion
  // codeset values with the translated codeset id from each factory that has
  // a matching native codeset. Those factories that do not have a matching
  // codeset are not retained in the list.
  int init_codeset_factories_i (TAO_CodesetFactorySet&,
                                CONV_FRAME::CodeSetComponent&);

  // get the translator between our ncs_c and the supplied tcs_c
  TAO_Codeset_Translator_Factory * get_char_trans (CONV_FRAME::CodeSetId);

  // get the translator between our ncs_w and the supplied tcs_w
  TAO_Codeset_Translator_Factory * get_wchar_trans (CONV_FRAME::CodeSetId);

  TAO_Codeset_Translator_Factory * get_translator_i (TAO_CodesetFactorySet&,
                                                     CONV_FRAME::CodeSetId);

  // The CodeSetComponentInfo struct contains all of the information regarding
  // the code sets this application recognizes. This is where the native code
  // set for both char and wchar are stored.
  CONV_FRAME::CodeSetComponentInfo codeset_info_;

  // The lists of available translators for both chars and wchars.
  TAO_CodesetFactorySet char_factories_;
  TAO_CodesetFactorySet wchar_factories_;
};

#include "ace/post.h"
#endif /* TAO_CODESET_MANAGER_H */
