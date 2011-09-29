// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_UCS2_Factory.h
 *
 *  $Id$
 *
 *  A tempate instantiation of a codeset translator factory from TAO. This
 *  loads the UTF16_UCS2_Translator.
 *
 *
 *  @author   Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef UTF16_UCS2_FACTORY_H
#define UTF16_UCS2_FACTORY_H

#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "UTF16_UCS2_export.h"
#include "UTF16_UCS2_Translator.h"

typedef TAO_Codeset_Translator_Factory_T<UTF16_UCS2_Translator> UTF16_UCS2_Factory;

ACE_STATIC_SVC_DECLARE_EXPORT (UTF16_UCS2, UTF16_UCS2_Factory)
ACE_FACTORY_DECLARE (UTF16_UCS2, UTF16_UCS2_Factory)

#endif /* UTF16_UCS2_FACTORY_H */

