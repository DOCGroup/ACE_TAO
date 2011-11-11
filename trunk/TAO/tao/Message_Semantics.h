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

struct TAO_Message_Semantics
  {
    enum Type
      {
        TAO_ONEWAY_REQUEST = 0,
        TAO_TWOWAY_REQUEST = 1,
        TAO_REPLY
      };

    enum Mode
      {
        TAO_SYNCH_MODE,
        TAO_ASYNCH_CALLBACK,
        TAO_ASYNCH_POLL
      };

    TAO_Message_Semantics ()
      : type_ (TAO_TWOWAY_REQUEST), mode_ (TAO_SYNCH_MODE) {}
    TAO_Message_Semantics (Type type)
      : type_ (type), mode_ (TAO_SYNCH_MODE) {}
    TAO_Message_Semantics (Type type, Mode mode)
      : type_ (type), mode_ (mode) {}
    TAO_Message_Semantics (const TAO_Message_Semantics& ms)
      : type_ (ms.type_), mode_ (ms.mode_) {}

    Type type_;
    Mode mode_;
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_MESSAGE_SEMANTICS_H */
