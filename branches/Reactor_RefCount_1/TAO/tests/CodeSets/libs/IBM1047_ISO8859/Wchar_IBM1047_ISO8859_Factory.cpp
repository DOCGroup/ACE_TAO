// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CodeSets/libs/IBM1047_ISO8859
//
// = FILENAME
//    Wchar_IBM1047_ISO8859_Factory.cpp
//
// = DESCRIPTION
//    A tempate instantiation of the codeset translator factory from TAO. This
//    one loads a modified version of ACE's IBM1047 (EBCDIC) to ISO8859 (ASCII)
//    translator. In this case, wchars and wstrings are translated rather than
//    strings and chars.
//
// = AUTHORS
//      Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "Wchar_IBM1047_ISO8859_Factory.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Codeset_Translator_Factory_T<WIBM1047_ISO8859>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Codeset_Translator_Factory_T<WIBM1047_ISO8859>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

ACE_STATIC_SVC_DEFINE (Wchar_IBM1047_ISO8859_Factory,
                       ACE_TEXT ("Wchar_IBM1047_ISO8859_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Wchar_IBM1047_ISO8859_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (IBM1047_ISO8859, Wchar_IBM1047_ISO8859_Factory)
