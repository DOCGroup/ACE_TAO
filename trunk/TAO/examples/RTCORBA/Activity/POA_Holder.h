/* -*- C++ -*- */
//=============================================================================
/**
 *  @file POA_Holder.h
 *
 *  $Id$
 *
 *  A helper to hold options for creating a POA.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================
#ifndef POA_HOLDER_H
#define POA_HOLDER_H

#include "tao/RTCORBA/RTCORBA.h"
#include "tao/PortableServer/PortableServer.h"
#include "activity_export.h"
#include "ace/SString.h"
#include "ace/Arg_Shifter.h"

/**
 * @class POA_Holder
 *
 * @brief An options holder for parameters to creating a poa.
 *
 */
class activity_Export POA_Holder
{
 public:
  /// Constructor
  POA_Holder (void);

  /// The arg_shifter options are read in the following manner: -POA <name> -PriorityModel <CLIENT|SERVER> <priority> -Lanes <count> (-Lane <priority> ,<static_threads> <dynamic_threads>)* -Bands <count> (-Band <low> <high>)*
  int init (ACE_Arg_Shifter& arg_shifter);

  /// Activate the new POA using the parameters initialized before.
  void activate (RTCORBA::RTORB_ptr rt_orb, PortableServer::POA_ptr parent_poa);

 protected:
  /// Cleanup.
  PortableServer::POA_var poa_;

  /// = POA create options.
  ACE_CString POA_name_;
  RTCORBA::PriorityModel priority_model_;
  RTCORBA::Priority server_priority_;
  RTCORBA::ThreadpoolLanes lanes_;
  RTCORBA::PriorityBands bands_;
};

#endif /* POA_HOLDER_H */
