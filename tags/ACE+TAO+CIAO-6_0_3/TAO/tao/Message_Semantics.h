// -*- C++ -*-

//=============================================================================
/**
 *  @file Message_Semantics.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_MESSAGE_SEMANTICS_H
#define TAO_MESSAGE_SEMANTICS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

enum TAO_Message_Semantics
  {
    TAO_ONEWAY_REQUEST = 0,
    TAO_TWOWAY_REQUEST = 1,
    TAO_REPLY
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_MESSAGE_SEMANTICS_H */
