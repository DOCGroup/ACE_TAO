// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table_Factory.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_OPERATION_TABLE_FACTORY_H
#define TAO_OPERATION_TABLE_FACTORY_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

class TAO_Operation_Table;

/**
 * @class TAO_Operation_Table_Factory
 *
 * @brief Factory for producing operation table lookup objects based on
 * the enumerated value of strategy held by the parameters.
 */
class TAO_PortableServer_Export TAO_Operation_Table_Factory
{
public:
  /// Return an instance of the specified lookup strategy
  TAO_Operation_Table *opname_lookup_strategy (void);

  /// Constructor
  TAO_Operation_Table_Factory (void);

  /// Destructor
  ~TAO_Operation_Table_Factory (void);
};

/// Define a singleton instance of the operation table factory.
typedef TAO_Singleton<TAO_Operation_Table_Factory,
                      TAO_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_FACTORY;

#include /**/ "ace/post.h"
#endif /* TAO_OPERATION_TABLE_FACTORY_H */
