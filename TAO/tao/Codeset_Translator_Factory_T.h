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

#include "ace/pre.h"
#include "tao/Codeset_Translator_Factory.h"

template<class NCS_TO_TCS>
class TAO_Export TAO_Codeset_Translator_Factory_T
: public TAO_Codeset_Translator_Factory
{
public:

  TAO_Codeset_Translator_Factory_T ();
  ~TAO_Codeset_Translator_Factory_T ();

  // initialize
  int init( int argc, ACE_TCHAR* argv[]);

  // ncs & tcs values set at compile time as a result of instantiating the
  // template.
  CONV_FRAME::CodeSetId ncs () const;
  CONV_FRAME::CodeSetId tcs () const;

  // set the apropriate translator (from_ for input to_ for output) to the CDR
  virtual void assign (TAO_InputCDR *) const;
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

#include "ace/post.h"
#endif /* TAO_Codeset_Translator_Factory_T */
