/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    POA_Guard.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_POA_GUARD_H
#define TAO_POA_GUARD_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ace/Guard_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_POA;

/**
 * @class TAO_POA_Guard
 *
 * @brief TAO_POA_Guard
 *
 * TAO_POA_Guard
 */
class TAO_PortableServer_Export TAO_POA_Guard
{
public:
  TAO_POA_Guard (TAO_POA &poa
                 ACE_ENV_ARG_DECL,
                 int check_for_destruction = 1);

private:
  ACE_Guard<ACE_Lock> guard_;
};

#include /**/ "ace/post.h"
#endif /* TAO_POA_GUARD_H */
