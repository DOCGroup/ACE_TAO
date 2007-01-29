// -*- C++ -*-
//=============================================================================
/**
 *  @file    WUCS4_UTF16_Factory.h
 *
 *  $Id$
 *
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef WUCS4_UTF16_FACTORY_H
#define WUCS4_UTF16_FACTORY_H

#include /**/ "ace/pre.h"
#include "ace/Service_Config.h"
#include "tao/Codeset/Codeset_Translator_Factory.h"

#include "WUCS4_UTF16_export.h"
#include "WUCS4_UTF16.h"

typedef TAO_Codeset_Translator_Factory_T<WUCS4_UTF16> WUCS4_UTF16_Factory;

ACE_STATIC_SVC_DECLARE_EXPORT (UCS4_UTF16, WUCS4_UTF16_Factory)
ACE_FACTORY_DECLARE (UCS4_UTF16, WUCS4_UTF16_Factory)

#include /**/ "ace/post.h"
#endif /* WUCS4_UTF16_FACTORY_H */
