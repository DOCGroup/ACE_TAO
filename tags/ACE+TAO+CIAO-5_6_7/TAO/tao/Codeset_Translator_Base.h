// -*- C++ -*-

// ===================================================================
/**
 *  @file   Codeset_Translator_Base.h
 *
 *  $Id$
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
// ===================================================================

#ifndef TAO_CODESET_TRANSLATOR_BASE_H
#define TAO_CODESET_TRANSLATOR_BASE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CONV_FRAMEC.h"
#include "tao/Codeset_Translator_Base.h"
#include "ace/Service_Object.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_InputCDR;
class TAO_OutputCDR;

// ****************************************************************

/**
 * @class TAO_Codeset_Translator_Base
 *
 * @brief Abstract base Translator component for interaction with TAO
 *
 * Certain elements of TAO need an interface defined in the TAO module
 * Through which the actual codeset translator instance may be assigned
 * to CDR streams.
 */

class TAO_Export TAO_Codeset_Translator_Base
{
public:
  virtual ~TAO_Codeset_Translator_Base ();
  virtual int init (int argc, ACE_TCHAR *argv[]) = 0;

  /// Get the native codeset ID from the base. There is no reasonable
  /// default for this method, so it is left abstract.
  virtual CONV_FRAME::CodeSetId ncs (void) const = 0;

  /// Get the translated codeset ID from the base. There is no reasonable
  /// default for this method, so it is left abstract.
  virtual CONV_FRAME::CodeSetId tcs (void) const = 0;

  /// Assign the translator to the supplied input CDR. This is left abstract
  /// since the base base does not have a reference to the actual translator
  /// instance. The template instance provides this implementation.
  virtual void assign (TAO_InputCDR *) const = 0;

  /// Assign the translator to the supplied output CDR. This is left abstract
  /// since the base base does not have a reference to the actual translator
  /// instance. The template instance provides this implementation.
  virtual void assign (TAO_OutputCDR *) const = 0;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_Codeset_Translator_Base */
