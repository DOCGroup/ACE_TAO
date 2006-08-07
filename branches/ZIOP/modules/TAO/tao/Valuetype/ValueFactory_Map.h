// -*- C++ -*-

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

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class ValueFactoryBase;
  typedef ValueFactoryBase *ValueFactory;
}

class TAO_ValueFactory_Map
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

  /// Return singleton instance of this class.
  static TAO_ValueFactory_Map * instance (void);

private:

  /// The hash table data structure.
  typedef ACE_Hash_Map_Manager_Ex<const char *,
                                  CORBA::ValueFactory,
                                  ACE_Hash<const char *>,
                                  ACE_Equal_To<const char *>,
                                  ACE_SYNCH_NULL_MUTEX>
          FACTORY_MAP_MANAGER;
  FACTORY_MAP_MANAGER map_;

  /// synchronization of the map
  TAO_SYNCH_MUTEX mutex_;
}; /* TAO_ValueFactory_Map */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_VALUEFACTORY_MAP_H */
