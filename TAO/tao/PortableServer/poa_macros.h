// $Id$

#ifndef TAO_POA_MACROS_H
#define TAO_POA_MACROS_H
#include "ace/pre.h"
#include "ace/CORBA_macros.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_OBJECT_ADAPTER_GUARD \
  ACE_GUARD_THROW_EX (ACE_Lock, \
                      monitor, \
                      this->lock (), \
                      CORBA::OBJ_ADAPTER ()); \
  ACE_CHECK

#define TAO_OBJECT_ADAPTER_GUARD_RETURN(RETURN) \
  ACE_GUARD_THROW_EX (ACE_Lock, \
                      monitor, \
                      this->lock (), \
                      CORBA::OBJ_ADAPTER ()); \
  ACE_CHECK_RETURN (RETURN)

#define TAO_POA_GUARD \
  TAO_POA_Guard poa_guard (*this TAO_ENV_ARG_PARAMETER); \
  ACE_UNUSED_ARG (poa_guard); \
  ACE_CHECK

#define TAO_POA_GUARD_RETURN(RETURN) \
  TAO_POA_Guard poa_guard (*this TAO_ENV_ARG_PARAMETER); \
  ACE_UNUSED_ARG (poa_guard); \
  ACE_CHECK_RETURN (RETURN)

#include "ace/post.h"
#endif /* TAO_POA_MACROS_H */
