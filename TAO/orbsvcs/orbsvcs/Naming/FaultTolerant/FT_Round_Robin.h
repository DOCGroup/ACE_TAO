// -*- C++ -*-

//=============================================================================
/**
 *  @file FT_Round_Robin.h
 *
 *  $Id$
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef FT_ROUND_ROBIN_H
#define FT_ROUND_ROBIN_H

#include /**/ "ace/pre.h"

#include "ace/Null_Mutex.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/FaultTolerant/FT_Location_Index_Map.h"


#include "orbsvcs/CosLoadBalancingS.h"
#include "ace/Vector_T.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

class TAO_FT_Naming_Manager;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_RoundRobin_Strategy
 *
 * @brief "Round Robin" load balancing strategy used by the
 * TAO_FT_Naming_Service.
 *
 * This load balancing strategy is designed to select an object group
 * member residing at the next location from an object group managed
 * by a specific Naming Manager. It selects an object from the object
 * group in the order in which they are stored.
 */
class TAO_FtNaming_Export TAO_FT_Round_Robin
{
public:

  /// Constructor.
  TAO_FT_Round_Robin (void);

  /// This function obtains the next object's location as it is bound
  /// within the object group.
  /// @param naming_manager The TAO_FT_Naming_Manager which houses the
  /// object groups.
  /// @param location The resulting location
  /// @param object_group The object group from which the object is to
  /// be selected
  /// @return False on error. Returns true if a valid object can
  /// be selected using the Round Robin load balancing strategy.
  virtual bool next_location (
      PortableGroup::ObjectGroup_ptr object_group,
      TAO_FT_Naming_Manager *naming_manager,
      PortableGroup::Location& location);

  /// Destructor
  virtual ~TAO_FT_Round_Robin (void);

private:

  /// Lock used to ensure atomic access to state retained by this
  /// class.
  TAO_SYNCH_MUTEX lock_;

  /// Table that maps PortableGroup::ObjectGroupId to location
  /// sequence index specific to a given object group.
  /**
   * The location sequence corresponds to the sequence containing the
   * locations of the members of a given object group.  The value
   * stored in this map corresponds to the index of the next element
   * in that sequence.  For example, if the index stored in the map is
   * 2, location[2] will be used when retrieving the object reference
   * to be returned from the Strategy::next_member() method.
   */
  TAO_FT_Location_Index_Map location_index_map_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_ROUND_ROBIN_H */
