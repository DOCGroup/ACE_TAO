// -*- C++ -*-
// $Id$

#include "WUCS4_UTF16_Factory.h"

ACE_STATIC_SVC_DEFINE (WUCS4_UTF16_Factory,
                       ACE_TEXT ("WUCS4_UTF16_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (WUCS4_UTF16_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (UCS4_UTF16, WUCS4_UTF16_Factory)
