// -*- C++ -*-

//=============================================================================
/**
 *  @file    Servant_Impl_Base.h
 *
 *  $Id$
 *
 *  This file contains the non-template declaration of a base class for
 *  the template mixin for the generated servant class.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef CIAO_SERVANT_IMPL_BASE_H
#define CIAO_SERVANT_IMPL_BASE_H

#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_EventC.h"
#include "CIAO_Server_Export.h"
#include "CCM_ComponentS.h"

namespace CIAO
{
  class Session_Container;

  /**
   * @class Servant_Impl_Base
   *
   * @brief Non-template base class for Servant_Impl.
   *
   * Holds the non-template parts of its child class
   * Servant_Impl.
   */
  class CIAO_SERVER_Export Servant_Impl_Base
  {
  public:
    Servant_Impl_Base (Session_Container * c);

    virtual ~Servant_Impl_Base (void);

    /// Operation to set attributes on the component.
    virtual void set_attributes (const Components::ConfigValues &descr
                                 ACE_ENV_ARG_DECL) = 0;

    //Creates and returns the StandardConfigurator for the component
    virtual Components::StandardConfigurator *get_standard_configurator (
        ACE_ENV_SINGLE_ARG_DECL
      );

        /// Override that returns the (passed-in) default POA of our member
    /// component's container, to ensure that we get registered
    /// to that POA when _this() is called.
    virtual PortableServer::POA_ptr _default_POA (
        ACE_ENV_SINGLE_ARG_DECL
      );

  protected:
    void add_facet (const char *port_name,
                    CORBA::Object_ptr port_ref);

    CORBA::Object_ptr lookup_facet (const char *port_name);

    void add_consumer (const char *port_name,
                       ::Components::EventConsumerBase_ptr port_ref);
                       
    ::Components::EventConsumerBase_ptr lookup_consumer (
        const char *port_name
      );
                       
  protected:
    typedef ACE_Hash_Map_Manager_Ex<const char *,
                                    ::Components::FacetDescription_var,
                                    ACE_Hash<const char *>,
                                    ACE_Equal_To<const char *>,
                                    ACE_Null_Mutex>
       FacetTable;

    typedef ACE_Hash_Map_Manager_Ex<const char *,
                                    ::Components::ConsumerDescription_var,
                                    ACE_Hash<const char *>,
                                    ACE_Equal_To<const char *>,
                                    ACE_Null_Mutex>
       ConsumerTable;

    FacetTable facet_table_;
    ConsumerTable consumer_table_;
    Session_Container * container_;
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_SERVANT_IMPL_T_H */
