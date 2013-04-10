// -*- C++ -*-

//=============================================================================
/**
 *  @file    UTF16_UCS2_Factory.cpp
 *
 *  $Id$
 *
 *  A tempate instantiation of the codeset translator factory from TAO.
 *
 *
 *  @author   Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================


#include "UTF16_UCS2_Factory.h"

ACE_STATIC_SVC_DEFINE (UTF16_UCS2_Factory,
                       ACE_TEXT ("UTF16_UCS2_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (UTF16_UCS2_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (UTF16_UCS2, UTF16_UCS2_Factory)
