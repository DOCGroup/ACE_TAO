// -*- C++ -*-
//=============================================================================
/**
 *  @file   Validate_Worker_T.h
 *
 *  $Id$
 *
 *  @author Yan Dai (dai_y@ociweb.com)
 */
//=============================================================================

#ifndef NOTIFY_VALIDATE_WORKER_H
#define NOTIFY_VALIDATE_WORKER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  /**
    * \brief Iterate through children reconnecting after reloading persistent information.
    */
  template<class TOPOOBJ>
  class Validate_Worker : public TAO_ESF_Worker<TOPOOBJ>
  {
  public:
    /// Constructor
    Validate_Worker();

    // override virtual ESF_Worker method
    virtual void work (TOPOOBJ* o);
  };
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Validate_Worker_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Validate_Worker_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* NOTIFY_VALIDATE_WORKER_H */


