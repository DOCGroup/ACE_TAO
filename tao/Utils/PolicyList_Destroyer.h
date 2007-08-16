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
#include /**/ "ace/pre.h"
#include "tao/Utils/utils_export.h"

#include "tao/PolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    class TAO_UTILS_Export PolicyList_Destroyer
      : public CORBA::PolicyList
    {
    public:
      PolicyList_Destroyer(CORBA::ULong length_hint);
      ~PolicyList_Destroyer() throw ();
    };

  } // namespace Utils
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Utils/PolicyList_Destroyer.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /*TAO_UTILS_POLICYLIST_DESTROYER_H*/
