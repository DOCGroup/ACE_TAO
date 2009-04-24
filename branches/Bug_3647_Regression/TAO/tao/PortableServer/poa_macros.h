// -*- C++ -*-

// $Id$

#ifndef TAO_POA_MACROS_H
#define TAO_POA_MACROS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_OBJECT_ADAPTER_GUARD \
  ACE_GUARD_THROW_EX (ACE_Lock, \
                      monitor, \
                      this->lock (), \
                      CORBA::OBJ_ADAPTER ())

#define TAO_OBJECT_ADAPTER_GUARD_RETURN(RETURN) \
  ACE_GUARD_THROW_EX (ACE_Lock, \
                      monitor, \
                      this->lock (), \
                      CORBA::OBJ_ADAPTER ())

#define TAO_POA_GUARD \
  TAO::Portable_Server::POA_Guard poa_guard (*this); \
  ACE_UNUSED_ARG (poa_guard)

#define TAO_POA_GUARD_RETURN(RETURN) \
  TAO::Portable_Server::POA_Guard poa_guard (*this); \
  ACE_UNUSED_ARG (poa_guard)

#include /**/ "ace/post.h"

#endif /* TAO_POA_MACROS_H */
