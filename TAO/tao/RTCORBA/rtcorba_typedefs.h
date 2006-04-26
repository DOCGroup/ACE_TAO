// -*- C++ -*-

//=============================================================================
/**
 *  @file rtcorba_typedefs.h
 *
 *  $Id$
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
  // TAO-specific typedefs
  typedef TAO_Priority_Mapping_Manager PriorityMappingManager;
  typedef TAO_Priority_Mapping_Manager *PriorityMappingManager_ptr;
  typedef TAO_Priority_Mapping_Manager_var PriorityMappingManager_var;
  typedef TAO_Priority_Mapping_Manager_out PriorityMappingManager_out;

  typedef TAO_Network_Priority_Mapping_Manager NetworkPriorityMappingManager;
  typedef TAO_Network_Priority_Mapping_Manager *NetworkPriorityMappingManager_ptr;
  typedef TAO_Network_Priority_Mapping_Manager_var NetworkPriorityMappingManager_var;
  typedef TAO_Network_Priority_Mapping_Manager_out NetworkPriorityMappingManager_out;

  typedef CORBA::Long NetworkPriority;

  typedef TAO_Priority_Mapping PriorityMapping;
  typedef TAO_Network_Priority_Mapping NetworkPriorityMapping;
  // End TAO-specific typedef
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_RTCORBA_TYPEDEFS_H*/
