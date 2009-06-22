// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Servant_State_Map.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_SERVANT_STATE_MAP_H
#define TAO_CSD_TP_SERVANT_STATE_MAP_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Servant_State.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class TP_Servant_State_Map
     *
     * @brief Map of Servant_State objects - one per servant.
     *
     * A TP_Stategy object holds an instance of a TP_Servant_State_Map object
     * as a (held-by-value) data member.  The strategy uses this map to
     * find or create the TP_Servant_State object for a particular servant
     * object.
     *
     */
    class TAO_CSD_TP_Export TP_Servant_State_Map
    {
    public:

      /// Default Constructor.
      TP_Servant_State_Map();

      /// Destructor.
      ~TP_Servant_State_Map();

      /// Accessor for the servant busy flag.
      TP_Servant_State* find(PortableServer::Servant servant
                            );

      /// Insert the servant to map.
      void insert(PortableServer::Servant servant
                 );

      /// Remove the servant from map.
      void remove(PortableServer::Servant servant
                 );


    private:

      /// Underlying Map Type - Hash-Based -
      /// Key Type: void*, Value Type: TP_Servant_State::HandleType
      typedef ACE_Hash_Map_Manager_Ex<void*,
                                      TP_Servant_State::HandleType,
                                      ACE_Hash<void*>,
                                      ACE_Equal_To<void*>,
                                      TAO_SYNCH_MUTEX>            MapType;

      /// The underlying map of servant state objects.
      MapType map_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Servant_State_Map.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_SERVANT_STATE_MAP_H */
