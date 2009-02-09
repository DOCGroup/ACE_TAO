// -*- C++ -*-

// ===================================================================
/**
 *  @file   Codeset_Translator_Factory.h
 *
 *  $Id$
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
// ===================================================================

#ifndef TAO_CODESET_TRANSLATOR_FACTORY_H
#define TAO_CODESET_TRANSLATOR_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"
#include "tao/Codeset_Translator_Base.h"
#include "tao/Codeset/codeset_export.h"


ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_InputCDR;
class TAO_OutputCDR;

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

class TAO_Codeset_Export TAO_Codeset_Translator_Factory :
  public TAO_Codeset_Translator_Base,
  public ACE_Service_Object
{
public:
  TAO_Codeset_Translator_Factory ();
  virtual ~TAO_Codeset_Translator_Factory ();
  virtual int init (int argc, ACE_TCHAR *argv[]);

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

TAO_END_VERSIONED_NAMESPACE_DECL

// Get the template includes last
#include "tao/Codeset/Codeset_Translator_Factory_T.h"

#include /**/ "ace/post.h"
#endif /* TAO_Codeset_Translator_Factory */
