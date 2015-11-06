// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_BOM_Factory.h
 *
 *  Loader for an instance of the UTF16_BOM_Translator.
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef UTF16_BOM_FACTORY_H
#define UTF16_BOM_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "tao/Codeset/UTF16_BOM_Translator.h"
#include "tao/Codeset/codeset_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Codeset_Export TAO_UTF16_BOM_Factory
  : public  TAO_Codeset_Translator_Factory
{
public:
  TAO_UTF16_BOM_Factory ();
  virtual ~TAO_UTF16_BOM_Factory ();
  virtual int init (int argc, ACE_TCHAR *argv[]);

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
  void create_translator () const;
  int parse_one_arg (int argc, ACE_TCHAR *argv[]);

private:
  TAO_UTF16_BOM_Translator *translator_;
  /// Force big endian wchar, warray, & wstring
  bool forceBE_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Codeset, TAO_UTF16_BOM_Factory)
ACE_FACTORY_DECLARE (TAO_Codeset, TAO_UTF16_BOM_Factory)
TAO_END_VERSIONED_NAMESPACE_DECL


#include /**/ "ace/post.h"
#endif /* UTF16_BOM_FACTORY_H */
