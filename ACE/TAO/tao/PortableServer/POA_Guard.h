// -*- C++ -*-

//=============================================================================
/**
 *  @file    POA_Guard.h
 *
 *  $Id$
 */
//=============================================================================

#ifndef TAO_POA_GUARD_H
#define TAO_POA_GUARD_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"
#include "ace/Guard_T.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Root_POA;

namespace TAO
{
  namespace Portable_Server
  {
    /**
     * @class POA_Guard
     *
     * @brief POA_Guard
     *
     * POA_Guard
     */
    class TAO_PortableServer_Export POA_Guard
    {
    public:
      POA_Guard (::TAO_Root_POA &poa
                 ACE_ENV_ARG_DECL,
                 int check_for_destruction = 1);

    private:
      ACE_Guard<ACE_Lock> guard_;
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_POA_GUARD_H */
