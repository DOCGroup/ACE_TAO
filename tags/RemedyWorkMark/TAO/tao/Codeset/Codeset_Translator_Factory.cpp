// -*- C++ -*-

//=============================================================================
/**
 *  @file    Codeset_Translator_Factory.cpp
 *
 *  $Id$
 *
 *  The base for all the translator factories. Translator factories are
 *  responsible for supplying the proper translator on demand.
 *
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#include "tao/Codeset/Codeset_Translator_Factory.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Codeset_Translator_Factory::TAO_Codeset_Translator_Factory ()
{

}

TAO_Codeset_Translator_Factory::~TAO_Codeset_Translator_Factory ()
{

}

int
TAO_Codeset_Translator_Factory::init (int , ACE_TCHAR **)
{
  return 0;
}

void
TAO_Codeset_Translator_Factory::assign_i (
    TAO_InputCDR *cdr,
    ACE_Char_Codeset_Translator *trans) const
{
  cdr->char_translator(trans);
}

void
TAO_Codeset_Translator_Factory::assign_i (
    TAO_OutputCDR *cdr,
    ACE_Char_Codeset_Translator *trans) const
{
  cdr->char_translator(trans);
}

void
TAO_Codeset_Translator_Factory::assign_i (
    TAO_InputCDR *cdr,
    ACE_WChar_Codeset_Translator *trans) const
{
  cdr->wchar_translator(trans);
}

void
TAO_Codeset_Translator_Factory::assign_i (
    TAO_OutputCDR *cdr,
    ACE_WChar_Codeset_Translator *trans) const
{
  cdr->wchar_translator(trans);
}

TAO_END_VERSIONED_NAMESPACE_DECL
