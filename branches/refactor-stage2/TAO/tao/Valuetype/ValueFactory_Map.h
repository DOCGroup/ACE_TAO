// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     ValueFactory_Map.h
 *
 *  $Id$
 *
 *  @author  Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef TAO_VALUEFACTORY_MAP_H
#define TAO_VALUEFACTORY_MAP_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/TAO_Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "valuetype_export.h"
#include "ace/Hash_Map_Manager_T.h"
//#include "ace/Synch.h"

class TAO_Valuetype_Export TAO_ValueFactory_Map
{
public:

  TAO_ValueFactory_Map (void);
  ~TAO_ValueFactory_Map (void);

  /**
   * Associate the factory (int_id) with the repo_id (ext_id).
   * Invokes _add_ref () on the factory.
   * If previous factory had been bind with this repo_id, this one is
   * returned in factory (and the caller holds a reference).
   * Returns -1 on failure, 0 on success and 1 if a previous factory
   * is found (and returned in factory).
   */
  int rebind (const char *repo_id, 
              CORBA::ValueFactory &factory);

  /// Removes entry for repo_id from the map and sets factory to
  /// the tied one.
  int unbind (const char *repo_id, 
              CORBA::ValueFactory &factory);

  /**
   * Lookup a matching factory for repo_id.
   * Invokes _add_ref () on the factory if found.
   * Returns -1 on failure and 0 on success.
   */
  int find (const char *repo_id, 
            CORBA::ValueFactory &factory);

  void dump (void);

private:

  /// The hash table data structure.
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  CORBA::ValueFactory,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  TAO_SYNCH_RW_MUTEX>
          FACTORY_MAP_MANAGER;
  FACTORY_MAP_MANAGER map_;
}; /* TAO_ValueFactory_Map */


// Currently the ValueFactory_Map is a singleton and not per ORB
// as in the OMG spec.
typedef TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX>
        TAO_VALUEFACTORY_MAP;

TAO_VALUETYPE_SINGLETON_DECLARE (TAO_Singleton,
                                 TAO_ValueFactory_Map,
                                 TAO_SYNCH_MUTEX)


#if defined (__ACE_INLINE__)
# include "ValueFactory_Map.inl"
#endif /* __ACE_INLINE__) */

#include "ace/post.h"

#endif /* TAO_VALUEFACTORY_MAP_H */
