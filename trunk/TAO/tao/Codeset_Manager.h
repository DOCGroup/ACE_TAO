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
// @@Phil/Mahesh -- We have a practice of including "ace/pre.h" and
// "ace/post.h" in the end. Could you folks please include that?

// @@Phil/Mahesh -- We include #pragma here. Please include that.
#include "tao/CONV_FRAMEC.h"

// @@Phil/Mahesh -- I dont see a need to include these two. Please
// forward declare the stuff.
#include "tao/Profile.h"
#include "tao/Transport.h"

class TAO_ServerRequest;
class TAO_Tagged_Components;
class TAO_Codeset_Transport_Factory;

// ****************************************************************

// @@Phil/Mahesh -- We need doxygen documentation/ intro for this
// class.

class TAO_Export TAO_Codeset_Item
{
public:
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

// @@Phil/Mahesh -- Any reason to have this declaration in global
// namespace in such a manner that any file that includes this
// file will get these declarations?

// typedefs for containers containing the list of codesets
// factories for character and wide character.
typedef ACE_Unbounded_Set<TAO_Codeset_Item*>
        TAO_CodesetFactorySet;

// Iterators
typedef ACE_Unbounded_Set_Iterator<TAO_Codeset_Item*>
        TAO_CodesetFactorySetItor;


// @@Phil/Mahesh -- We need doxygen documentation/ intro for this
// class.
class TAO_Export TAO_Codeset_Manager
{

public:
  /// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
  static CONV_FRAME::CodeSetId default_char_codeset;
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


  void set_ncs_c (CONV_FRAME::CodeSetId ncs);
  void set_ncs_w (CONV_FRAME::CodeSetId ncs, int maxbytes = 0);
  int add_char_translator (const char *name);
  int add_wchar_translator (const char *name);

  void configure_codeset_factories();

private:
  // Compute the TCS for Char/WChar asper the CORBA Specification
  CONV_FRAME::CodeSetId computeTCS (CONV_FRAME::CodeSetComponent &,
                                    CONV_FRAME::CodeSetComponent & );

  // Find CodesetId in the codeset component
  CORBA::Boolean isElementOf (CONV_FRAME::CodeSetId,
                              CONV_FRAME::CodeSetComponent & );

  // find the intersection of CodesetIds between Client and Server CCS
  CONV_FRAME::CodeSetId intersectionOf (CONV_FRAME::CodeSetComponent &,
                                        CONV_FRAME::CodeSetComponent &);

  CORBA::Boolean isCompatible (CONV_FRAME::CodeSetId,
                               CONV_FRAME::CodeSetId);


  int init_codeset_factories_i (TAO_CodesetFactorySet&,
                                CONV_FRAME::CodeSetComponent&);

  TAO_Codeset_Translator_Factory * get_char_trans (CONV_FRAME::CodeSetId,
                                                   CONV_FRAME::CodeSetId);

  TAO_Codeset_Translator_Factory * get_wchar_trans (CONV_FRAME::CodeSetId,
                                                    CONV_FRAME::CodeSetId);

  TAO_Codeset_Translator_Factory * get_translator_i (TAO_CodesetFactorySet&,
                                                     CONV_FRAME::CodeSetId,
                                                     CONV_FRAME::CodeSetId);

  CONV_FRAME::CodeSetComponentInfo codeset_info_;

  TAO_CodesetFactorySet char_factories_;
  TAO_CodesetFactorySet wchar_factories_;
};

#endif /* TAO_CODESET_MANAGER_H */
