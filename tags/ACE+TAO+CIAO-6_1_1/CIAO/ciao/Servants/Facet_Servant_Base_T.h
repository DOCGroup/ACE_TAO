// -*- C++ -*-

//=============================================================================
/**
 *  @file    Facet_Servant_Base_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated interface servant class.
 *
 *  @authors Marcel Smit <msmit@remedy.nl>
 */
//=============================================================================

#ifndef CIAO_FACET_INTERFACE_BASE_T_H
#define CIAO_FACET_INTERFACE_BASE_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Contexts/Context_Impl_Base_T.h"

namespace CIAO
{
  template <typename BASE, typename EXEC, typename CONTEXT>
  class Facet_Servant_Base_T
    : public virtual BASE
  {
  public:
    Facet_Servant_Base_T (typename EXEC::_ptr_type exec,
                          ::Components::CCMContext_ptr ctx);

    virtual ~Facet_Servant_Base_T (void);

    /// Get component implementation.
    virtual CORBA::Object_ptr _get_component (void);

  protected:
    typename EXEC::_var_type executor_;

    /// Context object.
    ::Components::CCMContext_var ctx_;
  };
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Facet_Servant_Base_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Facet_Servant_Base_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"


#endif /* CIAO_FACET_INTERFACE_BASE_T_H */
