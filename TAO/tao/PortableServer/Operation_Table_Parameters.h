// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Operation_Table_Parameters.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_OPERATION_TABLE_PARAMETERS_H
#define TAO_OPERATION_TABLE_PARAMETERS_H

#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Operation_Table;

#include "tao/TAO_Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

/**
 * @class TAO_Operation_Table_Parameters
 *
 * @brief Parameters used to create the operation table.
 */
class TAO_PortableServer_Export TAO_Operation_Table_Parameters
{
public:
  /// various lookup strategies
  enum DEMUX_STRATEGY
  {
    TAO_LINEAR_SEARCH,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_BINARY_SEARCH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
  };

  /// Set the lookup strategy from the list of enumerated values
  void lookup_strategy (DEMUX_STRATEGY s);

  /// Return the enumerated value for the lookup strategy. Default is
  /// Dynamic Hashing.
  DEMUX_STRATEGY lookup_strategy (void) const;

  /// Provide a data structure that will do the lookup. This is useful
  /// for user-defined lookup strategies.
  void concrete_strategy (TAO_Operation_Table *ot);

  /// Return the operation table that is being used to do the lookup.
  TAO_Operation_Table *concrete_strategy (void);

  /// Constructor.
  TAO_Operation_Table_Parameters (void);

  /// Destructor
  ~TAO_Operation_Table_Parameters (void);
private:
  /// Pointer to the object that implements a lookup strategy
  TAO_Operation_Table *strategy_;

  /// The enumerated value indicating the lookup strategy
  DEMUX_STRATEGY type_;
};

/// Define a singleton instance of operation table parameters.
typedef TAO_Singleton<TAO_Operation_Table_Parameters,
                      TAO_SYNCH_RECURSIVE_MUTEX>
        TAO_OP_TABLE_PARAMETERS;

#include /**/ "ace/post.h"
#endif /* TAO_OPERATION_TABLE_PARAMETERS_H */
