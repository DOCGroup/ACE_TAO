// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_BOM_Factory.cpp
 *
 *  $Id$
 *
 *  Loader for an instance of the UTF16_BOM_Translator.
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#include "tao/debug.h"
#include "tao/Codeset/UTF16_BOM_Factory.h"
#include "ace/OS_NS_strings.h"
#include "ace/Dynamic_Service.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (TAO_UTF16_BOM_Factory,
                       ACE_TEXT ("UTF16_BOM_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_UTF16_BOM_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Codeset, TAO_UTF16_BOM_Factory)



TAO_UTF16_BOM_Factory::TAO_UTF16_BOM_Factory ()
  : translator_ (0)
  , forceBE_ (false)
{
}

TAO_UTF16_BOM_Factory::~TAO_UTF16_BOM_Factory ()
{
  delete this->translator_;
}

int
TAO_UTF16_BOM_Factory::init (int argc, ACE_TCHAR *argv[])
{
  TAO_Codeset_Translator_Factory::init (argc, argv);

  for (int narg = 0; narg < argc;)
  {
    int consumed = parse_one_arg (argc - narg, &argv[narg]);
    if (consumed > 0)
    {
      narg += consumed;
    }
    else
    {
      TAOLIB_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t)TAO_UTF16_BOM_Factory parameter error: %s\n")
        ACE_TEXT ("Usage: TAO_UTF16_BOM_Factory \"-forceBE\"\n")
        , argv[narg]
        ));
      return -1;
    }
  }
  return 0;
}

int
TAO_UTF16_BOM_Factory::parse_one_arg (int argc, ACE_TCHAR *argv[])
{
  int consumed = 0;
  if ((argc > 0) && (ACE_OS::strcasecmp (argv[0], ACE_TEXT("-forcebe")) == 0))
  {
    this->forceBE_ = true;
    consumed = 1;
  }
  return consumed;
}

CONV_FRAME::CodeSetId
TAO_UTF16_BOM_Factory::ncs () const
{
  create_translator();
  return this->translator_->ncs();
}

CONV_FRAME::CodeSetId
TAO_UTF16_BOM_Factory::tcs () const
{
  create_translator();
  return this->translator_->tcs();
}

// Assign either a reference to the existing translator or a new translator
// for input CDR streams
void
TAO_UTF16_BOM_Factory::assign (TAO_InputCDR *cdr) const
{
  if (cdr)
    {
      create_translator ();
      this->assign_i (cdr, this->translator_);
    }
}

// Assign either a reference to the existing translator or a new translator
// for output CDR streams
void
TAO_UTF16_BOM_Factory::assign (TAO_OutputCDR *cdr) const
{
  if (cdr)
    {
      create_translator ();
      this->assign_i (cdr, this->translator_);
    }
}

void
TAO_UTF16_BOM_Factory::create_translator () const
{
  if (this->translator_ == 0)
  {
    TAO_UTF16_BOM_Factory * pthis = const_cast<TAO_UTF16_BOM_Factory *> (this);
    ACE_NEW (pthis->translator_, TAO_UTF16_BOM_Translator (this->forceBE_));
    if (this->translator_ == 0)
    {
      if (TAO_debug_level)
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) - UTF16_BOM_Factory: ")
                    ACE_TEXT ("Cannot create translator\n")
        ));
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
