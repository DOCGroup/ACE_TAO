// -*- C++ -*-
// $Id$

#include "tao/Transport_Selection_Guard.h"
#include "tao/TSS_Resources.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  Transport_Selection_Guard*
  Transport_Selection_Guard::current (TAO_ORB_Core* core, size_t tss_slot_id)
  {
    // @NOTE: (Iliyan) Started making this method aware of the core
    // and the tss slot that correspond to the "current" transport,
    // influenced by a general design preference to keep things
    // local. The idea was to make the current TSG part of the TSS
    // storage for a specific ORB Core, as opposed to using the global
    // TSS Resources. However, it really doesn't offer any benefit to
    // store a Transport pointer locally, for each ORB. There is
    // always only one current Transport per thread. Period. The
    // number of ORB Core instances in existence does not change that
    // fact, so keeping a separate pointer would have been an
    // over-kill.
    ACE_UNUSED_ARG (core);
    ACE_UNUSED_ARG (tss_slot_id);

#if TAO_HAS_TRANSPORT_CURRENT == 1
    return TAO_TSS_Resources::instance ()->tsg_;
#else  /* TAO_HAS_TRANSPORT_CURRENT != 1 */
    return 0;
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
  }

  /// Ctor
  Transport_Selection_Guard::Transport_Selection_Guard (TAO_Transport* t)
    :
#if TAO_HAS_TRANSPORT_CURRENT == 1
    prev_ (TAO_TSS_Resources::instance ()->tsg_) ,
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
    curr_ (t)
  {
#if TAO_HAS_TRANSPORT_CURRENT == 1
    TAO_TSS_Resources::instance ()->tsg_ = this;
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
  }

  /// Dtor
  Transport_Selection_Guard::~Transport_Selection_Guard (void)
  {
#if TAO_HAS_TRANSPORT_CURRENT == 1
    TAO_TSS_Resources::instance ()->tsg_ = prev_;
    this->prev_ = 0;
#endif  /* TAO_HAS_TRANSPORT_CURRENT == 1 */
    this->curr_ = 0;
  }

} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL
