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

#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton.h"
#include "ace/Synch.h"
#include "tao/corbafwd.h"

#ifdef TAO_HAS_VALUETYPE

class TAO_ValueFactory_Map
{
public:
  TAO_ValueFactory_Map ();
  ~TAO_ValueFactory_Map ();


  /**
   * Associate the factory (int_id) with the repo_id (ext_id).
   * Invokes _add_ref () on the factory.
   * If previous factory had been bind with this repo_id, this one is
   * returned in factory (and the caller holds a reference).
   * Returns -1 on failure, 0 on success and 1 if a previous factory
   * is found (and returned in factory).
   */
  int rebind (const char *repo_id, CORBA_ValueFactory_ptr &factory);

  /// Removes entry for repo_id from the map and sets factory to
  /// the tied one.
  int unbind (const char *repo_id, CORBA_ValueFactory_ptr &factory);

  /**
   * Lookup a matching factory for repo_id.
   * Invokes _add_ref () on the factory if found.
   * Returns -1 on failure and 0 on success.
   */
  int find (const char *repo_id, CORBA_ValueFactory_ptr &factory);

  void dump (void);
private:

  /// The hash table data structure.
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  CORBA_ValueFactory_ptr,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  TAO_SYNCH_RW_MUTEX>
          FACTORY_MAP_MANAGER;
  FACTORY_MAP_MANAGER map_;
}; /* TAO_ValueFactory_Map */


// currently the ValueFactory_Map is a singleton and not per ORB
// as in the OMG specs
typedef TAO_Singleton<TAO_ValueFactory_Map, TAO_SYNCH_MUTEX>
        TAO_VALUEFACTORY_MAP;



#if defined (__ACE_INLINE__)
# include "tao/ValueFactory_Map.i"
#endif /* __ACE_INLINE__) */

#endif /* TAO_HAS_VALUETYPE */
#include "ace/post.h"
#endif /* TAO_VALUEFACTORY_MAP_H */
