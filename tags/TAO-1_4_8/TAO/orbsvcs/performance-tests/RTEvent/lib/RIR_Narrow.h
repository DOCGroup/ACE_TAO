/**
 * @file RIR_Narrow.h
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */
#ifndef TAO_PERF_RTEC_RIR_NARROW_H
#define TAO_PERF_RTEC_RIR_NARROW_H

#include "tao/ORB.h"
#include "tao/Object.h"

/**
 * @class RIR_Narrow
 *
 * @brief Implement a helper method to narrow the results from
 *        resolve_initial_references.
 *
 */
template<class Interface>
class RIR_Narrow
{
public:
  typedef ACE_TYPENAME Interface::_ptr_type Interface_ptr;
  typedef ACE_TYPENAME Interface::_var_type Interface_var;

  static Interface_ptr resolve (CORBA::ORB_ptr orb,
                                const char *object_id
                                ACE_ENV_ARG_DECL);
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "RIR_Narrow.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_PERF_RTEC_RIR_NARROW_H */
