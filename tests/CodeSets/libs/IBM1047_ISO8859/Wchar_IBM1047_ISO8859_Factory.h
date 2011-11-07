// -*- C++ -*-

//=============================================================================
/**
 *  @file    Wchar_IBM1047_ISO8859_Factory.h
 *
 *  $Id$
 *
 *  A tempate instantiation of the codeset translator factory from TAO. This
 *  one loads a modified version of ACE's IBM1047 (EBCDIC) to ISO8859 (ASCII)
 *  translator. In this case, wchars and wstrings are translated rather than
 *  strings and chars.
 *
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef WCHAR_IBM1047_ISO8859_FACTORY_H
#define WCHAR_IBM1047_ISO8859_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "IBM1047_ISO8859_export.h"
#include "Wchar_IBM1047_ISO8859_Translator.h"

typedef TAO_Codeset_Translator_Factory_T<WIBM1047_ISO8859> Wchar_IBM1047_ISO8859_Factory;

ACE_STATIC_SVC_DECLARE_EXPORT (IBM1047_ISO8859, Wchar_IBM1047_ISO8859_Factory)
ACE_FACTORY_DECLARE (IBM1047_ISO8859, Wchar_IBM1047_ISO8859_Factory)

#include /**/ "ace/post.h"
#endif /* WCHAR_IBM1047_ISO8859_FACTORY_H */

