// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/*
 *  @file     Codeset_Manager.h
 *
 *  $Id$
 *
 *   Interface for the TAO CodeSet Manager.
 *
 *  @author  Mahesh Vedantam <mahesh#ociweb.com>
 */
//=============================================================================

#ifndef TAO_CODESET_MANAGER_H
#define TAO_CODESET_MANAGER_H

#include "tao/CONV_FRAMEC.h"
#include "tao/Profile.h"
#include "tao/Transport.h"

class TAO_ServerRequest;
class TAO_Tagged_Components;

class TAO_Export TAO_Codeset_Manager
{

public:
  /// NCS for char is defaulted to ISO 8859-1:1987; Latin Alphabet No. 1
  static CONV_FRAME::CodeSetId default_char_codeset;
  static CONV_FRAME::CodeSetId default_wchar_codeset;

  TAO_Codeset_Manager (CONV_FRAME::CodeSetComponentInfo* );

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
  void process_service_context ( TAO_ServerRequest &);

  /// Called by a client object to generate service context
  /// at this time Transport has the TCS for Char and WChar
  void generate_service_context ( TAO_Operation_Details&, TAO_Transport & );

private:
  ACE_UNIMPLEMENTED_FUNC ( TAO_Codeset_Manager() )

  // initialize the Codeset data members for characters and wide characters
    //  int init (const CONV_FRAME::CodeSetComponentInfo &);

  // Compute the TCS for Char/WChar asper the CORBA Specification
  CONV_FRAME::CodeSetId computeTCS (CONV_FRAME::CodeSetComponent &,
                                    CONV_FRAME::CodeSetComponent & );

  // Find CodesetId in the codeset component
  CORBA::Boolean isElementOf ( CONV_FRAME::CodeSetId,
                               CONV_FRAME::CodeSetComponent & );

  // find the intersection of CodesetIds between Client and Server CCS
  CONV_FRAME::CodeSetId intersectionOf (CONV_FRAME::CodeSetComponent &,
                                        CONV_FRAME::CodeSetComponent &);

  CORBA::Boolean isCompatible(CONV_FRAME::CodeSetId,
                              CONV_FRAME::CodeSetId);


  CONV_FRAME::CodeSetComponentInfo* codeset_info_;

protected:

};

#endif /* TAO_CODESET_MANAGER_H */
