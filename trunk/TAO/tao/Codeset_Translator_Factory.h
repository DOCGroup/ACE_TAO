// -*- C++ -*-

// ===================================================================
/**
 *  @file   TAO_Codeset_Translator_Factory.h
 *
 *  $Id$
 *
 *  @author  OCIWeb at St. Louis
 */
// =========================
#ifndef TAO_CODESET_TRANSLATOR_FACTORY_H
#define TAO_CODESET_TRANSLATOR_FACTORY_H

#include "ace/pre.h"
#include "ace/Service_Object.h"

#include "tao/corbafwd.h"
#include "tao/CONV_FRAMEC.h"

class TAO_InputCDR;
class TAO_OutputCDR;

class ACE_Char_Codeset_Translator;
class ACE_WChar_Codeset_Translator;

class TAO_Export TAO_Codeset_Translator_Factory :
  public ACE_Service_Object
{
public:
  TAO_Codeset_Translator_Factory ();
  virtual ~TAO_Codeset_Translator_Factory ();
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // ncs & tcs values set at compile time as a result of instantiating the
  // template.
  virtual CONV_FRAME::CodeSetId ncs () const = 0;
  virtual CONV_FRAME::CodeSetId tcs () const = 0;

  virtual void assign(TAO_InputCDR *) const = 0;
  virtual void assign (TAO_OutputCDR *) const = 0;

protected:
  void assign_i (TAO_InputCDR *, ACE_Char_Codeset_Translator* ) const;
  void assign_i (TAO_InputCDR *, ACE_WChar_Codeset_Translator* ) const;
  void assign_i (TAO_OutputCDR *, ACE_Char_Codeset_Translator* ) const;
  void assign_i (TAO_OutputCDR *, ACE_WChar_Codeset_Translator* ) const;
};

// Get the template includes last
#include "tao/Codeset_Translator_Factory_T.h"

#include "ace/post.h"
#endif /* TAO_Codeset_Translator_Factory */
