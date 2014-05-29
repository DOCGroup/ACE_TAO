// -*- C++ -*-

//=============================================================================
/**
 *  @file FT_Random.h
 *
 *  $Id$
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef FT_RANDOM_H
#define FT_RANDOM_H

#include /**/ "ace/pre.h"

#include "ace/Null_Mutex.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/FaultTolerant/FT_Location_Index_Map.h"


#include "orbsvcs/CosLoadBalancingS.h"
#include "ace/Vector_T.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Manager;

/**
 * @class TAO_LB_Random_Strategy
 *
 * @brief "Random" load balancing strategy used by the
 * TAO_FT_Naming_Service.
 *
 * This load balancing strategy is designed to select an object group
 * member residing at the next location from an object group managed
 * by a specific Naming Manager. It selects an object from the object
 * group in the order in which they are stored.
 */
class TAO_FtNaming_Export TAO_FT_Random
{
public:

  /// Constructor.
  TAO_FT_Random (void);

  /// Destructor
  virtual ~TAO_FT_Random (void);

  /// This function obtains the next object's location as it is bound
  /// within the object group.
  /// @param naming_manager The TAO_FT_Naming_Manager which houses the
  /// object groups.
  /// @param location The resulting location
  /// @param object_group The object group from which the object is to
  /// be selected
  /// @return False on error. Returns true if a valid object can
  /// be selected using the Random load balancing strategy.
  virtual bool next_location (
      PortableGroup::ObjectGroup_ptr object_group,
      TAO_FT_Naming_Manager *naming_manager,
      PortableGroup::Location& location);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_RANDOM_H */
