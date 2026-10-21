// -*- C++ -*-

//=============================================================================
/**
 *  @file rtcorba_typedefs.h
 *
 *  TAO specific typedef's moved away from the generated code.
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_RTCORBA_TYPEDEFS_H
#define TAO_RTCORBA_TYPEDEFS_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward references.
class TAO_Priority_Mapping_Manager;
class TAO_Priority_Mapping_Manager_var;
class TAO_Priority_Mapping_Manager_out;
class TAO_Priority_Mapping;
class TAO_Network_Priority_Mapping_Manager;
class TAO_Network_Priority_Mapping_Manager_var;
class TAO_Network_Priority_Mapping_Manager_out;
class TAO_Network_Priority_Mapping;

namespace RTCORBA
{
  // TAO-specific aliases
  using PriorityMappingManager = TAO_Priority_Mapping_Manager;
  using PriorityMappingManager_ptr = TAO_Priority_Mapping_Manager *;
  using PriorityMappingManager_var = TAO_Priority_Mapping_Manager_var;
  using PriorityMappingManager_out = TAO_Priority_Mapping_Manager_out;

  using NetworkPriorityMappingManager = TAO_Network_Priority_Mapping_Manager;
  using NetworkPriorityMappingManager_ptr = TAO_Network_Priority_Mapping_Manager *;
  using NetworkPriorityMappingManager_var = TAO_Network_Priority_Mapping_Manager_var;
  using NetworkPriorityMappingManager_out = TAO_Network_Priority_Mapping_Manager_out;

  using NetworkPriority = CORBA::Long;

  using PriorityMapping = TAO_Priority_Mapping;
  using NetworkPriorityMapping = TAO_Network_Priority_Mapping;
  // End TAO-specific aliases
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_RTCORBA_TYPEDEFS_H*/
