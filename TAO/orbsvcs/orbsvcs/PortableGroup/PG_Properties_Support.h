/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Support.h
 *
 *  $Id$
 *
 *  This file declares classes to help manage the PortableGroup::Properties
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_SUPPORT_H
#define TAO_PG_PROPERTIES_SUPPORT_H

#include "PG_Properties_Decoder.h"

namespace TAO
{
  class TAO_PortableGroup_Export PG_Properties_Support
  {
    typedef ACE_Hash_Map_Manager<
      ACE_CString,
      ::TAO_PG::Properties_Decoder *,
      TAO_SYNCH_MUTEX> Properties_Map;
    typedef ACE_Hash_Map_Iterator<
      ACE_CString,
      ::TAO_PG::Properties_Decoder *,
      TAO_SYNCH_MUTEX> Properties_Map_Iterator;

  public:
    PG_Properties_Support ();
    ~PG_Properties_Support ();

    void set_default_properties (const PortableGroup::Properties & props);

    PortableGroup::Properties * get_default_properties (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ( (CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty));

    void remove_default_properties (
        const PortableGroup::Properties & props
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void set_type_properties (
        const char *type_id,
        const PortableGroup::Properties & overrides
        ACE_ENV_ARG_DECL)  
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableGroup::InvalidProperty,
        PortableGroup::UnsupportedProperty));

    PortableGroup::Properties * get_type_properties (
        const char *type_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ( (CORBA::SystemException));

    TAO_PG::Properties_Decoder *  TAO::PG_Properties_Support::find_typeid_properties (
        const char *type_id
        ACE_ENV_ARG_PARAMETER)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void remove_type_properties (
        const char *type_id,
        const PortableGroup::Properties & props
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ( (CORBA::SystemException));

    ///////////////
    // Data Members
  private:
    /**
     * Protect internal state.
     */
    TAO_SYNCH_MUTEX internals_;
    typedef ACE_Guard<TAO_SYNCH_MUTEX> InternalGuard;

    TAO_PG::Properties_Decoder default_properties_;
    Properties_Map properties_map_;
  };
} //namespace TAO_PG

#endif // TAO_PG_PROPERTIES_SUPPORT_H
