// -*- C++ -*-

// ===================================================================
/**
 *  @file   Codeset_Translator_Factory.h
 *
 *  $Id$
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
// =========================
#ifndef TAO_CODESET_TRANSLATOR_FACTORY_H
#define TAO_CODESET_TRANSLATOR_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"

class TAO_InputCDR;
class TAO_OutputCDR;

class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;


// ****************************************************************

/**
 * @class TAO_Codeset_Translator_Factory
 *
 * @brief Abstract base class for factories providing codeset translators
 *
 * The codeset translator factory is a loadable service object. It is used to
 * supply the actual translator used in converting between two codesets. The
 * intent of using a factory is to avoid requiring codeset translators to be
 * multiply inherited both from the translator base class and the service
 * object base. The translator factory is also responsible for assigning
 * translators to CDR streams. Since there is no common base class between
 * input and output CDRs, the assingment code must be duplicated.
 */

class TAO_Export TAO_Codeset_Translator_Factory :
  public ACE_Service_Object
{
public:
  TAO_Codeset_Translator_Factory ();
  virtual ~TAO_Codeset_Translator_Factory ();
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Get the native codeset ID from the factory. There is no reasonable
  /// default for this method, so it is left abstract.
  virtual CONV_FRAME::CodeSetId ncs () const = 0;

  /// Get the translated codeset ID from the factory. There is no reasonable
  /// default for this method, so it is left abstract.
  virtual CONV_FRAME::CodeSetId tcs () const = 0;

  /// Assign the translator to the supplied input CDR. This is left abstract
  /// since the factory base does not have a reference to the actual translator
  /// instance. The template instance provides this implementation.
  virtual void assign (TAO_InputCDR *) const = 0;

  /// Assign the translator to the supplied output CDR. This is left abstract
  /// since the factory base does not have a reference to the actual translator
  /// instance. The template instance provides this implementation.
  virtual void assign (TAO_OutputCDR *) const = 0;

protected:

  /// Assign the translator to the supplied input CDR. The template instance
  /// will have a translator that is based on either the Char or Wchar
  /// translator, so the compiler will select the appropriate call from
  /// assign().
  void assign_i (TAO_InputCDR *, ACE_Char_Codeset_Translator* ) const;
  /// Assign the translator to the supplied input CDR. The template instance
  /// will have a translator that is based on either the Char or Wchar
  /// translator, so the compiler will select the appropriate call from
  /// assign().
  void assign_i (TAO_InputCDR *, ACE_WChar_Codeset_Translator* ) const;
  /// Assign the translator to the supplied output CDR. The template instance
  /// will have a translator that is based on either the Char or Wchar
  /// translator, so the compiler will select the appropriate call from
  /// assign().
  void assign_i (TAO_OutputCDR *, ACE_Char_Codeset_Translator* ) const;
  /// Assign the translator to the supplied output CDR. The template instance
  /// will have a translator that is based on either the Char or Wchar
  /// translator, so the compiler will select the appropriate call from
  /// assign().
  void assign_i (TAO_OutputCDR *, ACE_WChar_Codeset_Translator* ) const;
};

// Get the template includes last
#include "tao/Codeset_Translator_Factory_T.h"

#include "ace/post.h"
#endif /* TAO_Codeset_Translator_Factory */
