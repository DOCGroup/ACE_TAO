// -*- C++ -*-
//=============================================================================
/**
 * @file PolicyList_Destroyer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_POLICYLIST_DESTROYER_H
#define TAO_UTILS_POLICYLIST_DESTROYER_H
#include "ace/pre.h"

#include "tao/PolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Utils
  {

    /**
     * @class PolicyList_Destroyer
     *
     * @brief Automatically destroy all the policies set in a PolicyList
     *
     */
    class PolicyList_Destroyer
      : public CORBA::PolicyList
    {
    public:
      PolicyList_Destroyer(CORBA::ULong length_hint);
      ~PolicyList_Destroyer()
        ACE_THROW_SPEC (());
    };

  } // namespace Utils
} // namespace TAO

#if defined (__ACE_INLINE__)
# include "PolicyList_Destroyer.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_UTILS_POLICYLIST_DESTROYER_H*/
