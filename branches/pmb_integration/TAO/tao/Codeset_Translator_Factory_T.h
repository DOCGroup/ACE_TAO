// -*- C++ -*-

// ===================================================================
/**
 *  @file   Codeset_Translator_Factory_T.h
 *
 *  $Id$
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
// =========================
#ifndef TAO_CODESET_TRANSLATOR_FACTORY_T_H
#define TAO_CODESET_TRANSLATOR_FACTORY_T_H

#include /**/ "ace/pre.h"

#include "tao/Codeset_Translator_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Codeset_Translator_Factory_T
 *
 * @brief Template for translator factory classes.
 *
 * The template argument is the actual translator class. The factory creates an
 * instance of the translator during initialization. Other than that, the
 * template returns the actual values for the native and translated codeset
 * ids, and performs the translator assignment to the CDR objects as needed.
 */

template<class NCS_TO_TCS>
class TAO_Codeset_Translator_Factory_T
: public TAO_Codeset_Translator_Factory
{
public:

  TAO_Codeset_Translator_Factory_T ();
  ~TAO_Codeset_Translator_Factory_T ();

  /// initialize the factory service object. Instantiates the translator.
  int init( int argc, ACE_TCHAR* argv[]);

  /// ncs returns the translator's native codeset ID.
  CONV_FRAME::CodeSetId ncs () const;
  /// tcs returns the translator's transmission codeset ID.
  CONV_FRAME::CodeSetId tcs () const;

  /// Assign the translator to the input CDR. The inherited assign_i is used
  /// to assign either a char or wchar translator, depending on the base type
  /// of NCS_TO_TCS. A null input CDR is permitted, in which case assign is a
  /// no-op.
  virtual void assign (TAO_InputCDR *) const;
  /// Assign the translator to the output CDR. The inherited assign_i is used
  /// to assign either a char or wchar translator, depending on the base type
  /// of NCS_TO_TCS. A null output CDR is permitted, in which case assign is a
  /// no-op.
  virtual void assign (TAO_OutputCDR *) const;

private:
  NCS_TO_TCS *translator_;
};

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Codeset_Translator_Factory_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Codeset_Translator_Factory_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Codeset_Translator_Factory_T */
