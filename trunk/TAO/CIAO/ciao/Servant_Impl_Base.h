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

  protected:
    void add_facet (const char *port_name,
                    CORBA::Object_ptr port_ref);
                    
    void add_consumer (const char *port_name,
                       ::Components::EventConsumerBase_ptr port_ref);

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

