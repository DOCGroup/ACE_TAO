// -*- C++ -*-

//=============================================================================
/**
*  \file    Save_Persist_Worker_T.h
*
*  $Id$
*
*  \author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef SAVE_PERSIST_WORKER_H
#define SAVE_PERSIST_WORKER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  class Topology_Saver;

  /**
   * \brief Iterate through children saving persistent topology information.
   */
  template<class TOPOOBJ>
  class Save_Persist_Worker : public TAO_ESF_Worker<TOPOOBJ>
  {
  public:
    /**
     * /brief Constructor
     * /param saver the object that will save persistent information.
     * /param want_all_children if true, unmodified children should be saved, too.
     */
    Save_Persist_Worker(Topology_Saver& saver, bool want_all_children);

    // override virtual ESF_Worker method
    virtual void work (TOPOOBJ* o);

  private:
    Topology_Saver& saver_;
    bool want_all_children_;
  };
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Save_Persist_Worker_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Save_Persist_Worker_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* SAVE_PERSIST_WORKER_H */
