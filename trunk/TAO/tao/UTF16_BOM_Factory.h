// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    UTF16_BOM_Factory.h
//
// = DESCRIPTION
//    Loader for an instance of the UTF16_BOM_Translator.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#ifndef UTF16_BOM_FACTORY_H
#define UTF16_BOM_FACTORY_H

#include <ace/pre.h>
#include <ace/Service_Config.h>
#include <tao/Codeset_Translator_Factory.h>

#include "UTF16_BOM_Translator.h"

typedef TAO_Export TAO_Codeset_Translator_Factory_T<UTF16_BOM_Translator> UTF16_BOM_Factory;

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, UTF16_BOM_Factory)
ACE_FACTORY_DECLARE (TAO, UTF16_BOM_Factory)

#include <ace/post.h>
#endif /* UTF16_BOM_FACTORY_H */
