// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    tao
//
// = FILENAME
//    UTF16_BOM_Factory.cpp
//
// = DESCRIPTION
//    Loader for an instance of the UTF16_BOM_Translator.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "UTF16_BOM_Factory.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Codeset_Translator_Factory_T<UTF16_BOM_Translator>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Codeset_Translator_Factory_T<UTF16_BOM_Translator>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_STATIC_SVC_DEFINE (UTF16_BOM_Factory,
                       ACE_TEXT ("UTF16_BOM_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (UTF16_BOM_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, UTF16_BOM_Factory)
