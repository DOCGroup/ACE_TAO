
//=============================================================================
/**
 *  @file    Codeset_Translator_Factory_T.cpp
 *
 *  $Id$
 *
 *  The template for creating a particular instance of a codeset translator
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CODESET_TRANSLATOR_FACTORY_T_CPP
#define TAO_CODESET_TRANSLATOR_FACTORY_T_CPP

#include "tao/Codeset/Codeset_Translator_Factory_T.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class NCS_TO_TCS>
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::TAO_Codeset_Translator_Factory_T () :
  translator_(0)
{
}

template<class NCS_TO_TCS>
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::~TAO_Codeset_Translator_Factory_T ()
{
  delete translator_;
}

template<class NCS_TO_TCS>
int
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::init (int argc, ACE_TCHAR* argv[])
{
  this->TAO_Codeset_Translator_Factory::init (argc,argv);
  ACE_NEW_RETURN (translator_,NCS_TO_TCS,-1);
  if( TAO_debug_level > 0 )
    TAOLIB_DEBUG((LM_DEBUG, "TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::init() entered\n"));
  return 0;
}

// ncs & tcs values set at compile time as a result of instantiating the
// template.
template<class NCS_TO_TCS>
CONV_FRAME::CodeSetId
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::ncs () const
{
  return this->translator_->ncs();
}

template<class NCS_TO_TCS>
CONV_FRAME::CodeSetId
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::tcs () const
{
  return this->translator_->tcs();
}

// Assign either a reference to teh existing translator or a new translator
// for input CDR streams
template<class NCS_TO_TCS>
void
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::assign (TAO_InputCDR *cdr) const
{
  if (cdr)
    {
      this->assign_i(cdr,this->translator_);
    }
}

// Assign either a reference to teh existing translator or a new translator
// for input CDR streams
template<class NCS_TO_TCS>
void
TAO_Codeset_Translator_Factory_T<NCS_TO_TCS>::assign (TAO_OutputCDR *cdr) const
{
  if (cdr)
    {
      this->assign_i(cdr,this->translator_);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_CODESET_TRANSLATOR_FACTORY_T_CPP */
