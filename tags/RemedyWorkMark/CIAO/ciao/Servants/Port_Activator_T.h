// -*- C++ -*-

//=============================================================================
/**
 *  @file    Port_Activator_T.h
 *
 *  $Id$
 *
 *  @authors Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_PORT_ACTIVATOR_T_H
#define CIAO_PORT_ACTIVATOR_T_H
#include /**/ "ace/pre.h"

#include "ciao/Servants/Port_Activator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO
{
  /**
   * @class Port_Activator_T
   *
   * @brief
   * Concrete class that implements the strategy for creating
   * the right type of servant for the ports in question.
   *
   * This class is parametrized by the servant type for the port, the
   * executor type for the port, the context for the component and the
   * component servant which instantiated this class within the
   * container.
   */
  template <typename SERV,
            typename EXEC,
            typename CONTEXT,
            typename COMP_SERV>
  class Port_Activator_T : public virtual Port_Activator_i
  {
  public:
    typedef SERV SERVANT;

    Port_Activator_T (const char *oid,
                      const char *name,
                      Port_Activator_Types::Type t,
                      typename EXEC::_ptr_type e,
                      typename CONTEXT::_ptr_type c,
                      COMP_SERV *cs);

    /// Template method from the base class, please see the base class
    /// documentation for details.
    virtual PortableServer::Servant activate (
      const PortableServer::ObjectId &oid);

    virtual void deactivate (
      PortableServer::Servant servant, CORBA::Boolean remaining_activations);

  private:
    /// The executor
    typename EXEC::_var_type executor_;

    /// Context classes
    typename CONTEXT::_var_type context_;

    /// Component servant which created @c this
    COMP_SERV *comp_serv_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Port_Activator_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Port_Activator_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /*CIAO_SERVANT_ACTIVATOR_T_H*/
