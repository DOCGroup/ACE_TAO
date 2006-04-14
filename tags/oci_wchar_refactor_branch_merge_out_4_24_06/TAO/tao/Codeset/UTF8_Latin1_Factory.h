// -*- C++ -*-
// $Id$

#ifndef UTF8_LATIN1_FACTORY_H
#define UTF8_LATIN1_FACTORY_H

#include /**/ "ace/pre.h"
#include "codeset_export.h"
#include "ace/Service_Config.h"
#include "Codeset_Translator_Factory.h"

#include "UTF8_Latin1_Translator.h"

class TAO_Codeset_Export TAO_UTF8_Latin1_Factory
  : public  TAO_Codeset_Translator_Factory
{
public:
  TAO_UTF8_Latin1_Factory ();
  virtual ~TAO_UTF8_Latin1_Factory ();
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

private:
  TAO_UTF8_Latin1_Translator *translator_;
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Codeset, TAO_UTF8_Latin1_Factory)
ACE_FACTORY_DECLARE (TAO_Codeset, TAO_UTF8_Latin1_Factory)

#include /**/ "ace/post.h"
#endif /* UTF8_LATIN1_FACTORY_H */
