// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Impl_T.h
 *
 *  $Id$
 *
 *  This file contains the declaration of a mixin base class for
 *  the generated servant class.
 *
 *  @authors Boris Kolpackov <boris@dre.vanderbilt.edu>
 *           Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_SERVANT_IMPL_T_H
#define CIAO_SERVANT_IMPL_T_H

#include /**/ "ace/pre.h"

#include "Servant_Impl_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  class Session_Container;

  /**
   * @class Servant_Impl
   *
   * @brief Mixin base class for generated servant.
   *
   * This class implements navigation and other operations
   * common to all generated servants.
   */
  template <typename SKEL, typename EXEC, typename CONTEXT>
  class Servant_Impl : public virtual SKEL,
                       public Servant_Impl_Base
  {
  public:
    Servant_Impl (typename EXEC::_ptr_type exe, 
                  Session_Container * c);
    virtual ~Servant_Impl (void);
   
    virtual ::Components::FacetDescriptions *
    get_all_facets (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  
    virtual ::Components::ConsumerDescriptions *
    get_all_consumers (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
      
  protected: 
    typename EXEC::_var_type executor_;        
    CONTEXT * context_;  
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Servant_Impl_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Servant_Impl_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_SERVANT_IMPL_T_H */
