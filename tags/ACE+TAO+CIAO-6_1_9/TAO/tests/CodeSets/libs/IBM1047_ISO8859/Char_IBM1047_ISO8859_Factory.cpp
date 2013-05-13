// -*- C++ -*-

//=============================================================================
/**
 *  @file    Char_IBM1047_ISO8859_Factory.cpp
 *
 *  $Id$
 *
 *  A tempate instantiation of the codeset translator factory from TAO. This
 *  one loads a modified version of ACE's IBM1047 (EBCDIC) to ISO8859 (ASCII)
 *  translator.
 *
 *
 *  @author   Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#include "Char_IBM1047_ISO8859_Factory.h"
#include "ace/Log_Msg.h"

ACE_STATIC_SVC_DEFINE (Char_IBM1047_ISO8859_Factory,
                       ACE_TEXT ("Char_IBM1047_ISO8859_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Char_IBM1047_ISO8859_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (IBM1047_ISO8859, Char_IBM1047_ISO8859_Factory)
