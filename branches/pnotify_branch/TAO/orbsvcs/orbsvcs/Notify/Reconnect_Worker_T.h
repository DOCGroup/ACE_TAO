/* -*- C++ -*- */

//=============================================================================
/**
*  @file    Reconnect_Worker_T.h
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef RECONNECT_WORKER_H
#define RECONNECT_WORKER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO_NOTIFY
{
  /**
   * \brief Iterate through children reconnecting after reloading persistent information.
   */
  template<class TOPOOBJ>
  class Reconnect_Worker : public TAO_ESF_Worker<TOPOOBJ>
  {
  public:
    /// Constructor
    Reconnect_Worker();

    // override virtual ESF_Worker method
    virtual void work (TOPOOBJ* o ACE_ENV_ARG_DECL);
  };
} // namespace TAO_NOTIFY

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Reconnect_Worker_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Reconnect_Worker_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* RECONECT_WORKER_H */
