/* -*- C++ -*- */


//=============================================================================
/**
 *  @file   Endpoint_Strategy.h
 *
 *  $Id$
 *
 *  @author Sumedh Mungee <sumedh@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_ENDPOINT_STRATEGY_H
#define TAO_AV_ENDPOINT_STRATEGY_H
#include /**/ "ace/pre.h"

#include "AVStreams_i.h"
#include "tao/PortableServer/ORB_Manager.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

/**
 * @class TAO_AV_Endpoint_Strategy
 *
 * Base class to define various endpoint strategies
 * used by the MMDevice to create the Endpoint and Vdev
 */
class TAO_AV_Export TAO_AV_Endpoint_Strategy
{

public:
  /// Constructor
  TAO_AV_Endpoint_Strategy (void);

  /// Destructor
  virtual ~TAO_AV_Endpoint_Strategy (void);

  /// Called by the MMDevice, when it needs to create an A type endpoint
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);

  /// Called by the MMDevice, when it needs to create an B type endpoint
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);

protected:
  /// The "A" stream endpoint
  AVStreams::StreamEndPoint_A_var stream_endpoint_a_;

  /// The "B" stream endpoint
  AVStreams::StreamEndPoint_B_var stream_endpoint_b_;

  /// The vdev
  AVStreams::VDev_var vdev_;

};

// ----------------------------------------------------------------------
/**
 * @class TAO_AV_Endpoint_Process_Strategy
 * @brief Process-based strategy for creating endpoints.
 *        Abstract base class.
 */
class TAO_AV_Export TAO_AV_Endpoint_Process_Strategy
  : public TAO_AV_Endpoint_Strategy
{

public:
  /// Constructor. The process_options contain the name and arguments
  /// for the process to be created
  TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Process_Strategy (void);

  /// creates a new child process, and waits on a semaphore
  /// until the child process has finished creating the endpoints
  virtual int activate (void);

protected:
  /// Bind to the naming service
  virtual int bind_to_naming_service (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * Get the object reference for the newly created stream
   * endpoint (which will be in the child process)
   * Subclasses will define the functionality for this
   */
  virtual int get_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// Get the Vdev object reference for the newly created
  /// endpoint
  virtual int get_vdev (ACE_ENV_SINGLE_ARG_DECL);

  /// Naming context
  CosNaming::NamingContext_var naming_context_;

  /// Name and arguments for the process to be created
  ACE_Process_Options *process_options_;

  /// name of this host used for resolving unique names.
  char host_[MAXHOSTNAMELEN];

  /// My child's process id.
  pid_t pid_;
};


// ----------------------------------------------------------------------

/**
 * @class TAO_AV_Endpoint_Process_Strategy_A
 * @brief Process-based strategy to create "A" type endpoints
 */
class TAO_AV_Export TAO_AV_Endpoint_Process_Strategy_A
  : public TAO_AV_Endpoint_Process_Strategy
{

public:
  /// Constructor
  TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Process_Strategy_A (void);

protected:
  /// Creates an "A" type stream endpoint, and a vdev
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);

  /// Gets the "A" type stream endpoint from the child process
  virtual int get_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL);

};

// ----------------------------------------------------------------------

/**
 * @class TAO_AV_Endpoint_Process_Strategy_B
 * @brief Process-based strategy to create "B" type endpoints
 */
class TAO_AV_Export TAO_AV_Endpoint_Process_Strategy_B
  : public TAO_AV_Endpoint_Process_Strategy
{

public:
  /// Constructor
  TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options);

  /// Destructor.
  virtual ~TAO_AV_Endpoint_Process_Strategy_B (void);

protected:
  /// Creates a "B" type stream endpoint, and a vdev
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev
                        ACE_ENV_ARG_DECL);


  /// Gets the object reference of the "B" type streamendpoint.
  virtual int get_stream_endpoint (ACE_ENV_SINGLE_ARG_DECL);

};

// Include the templates here.
#include "Endpoint_Strategy_T.h"

#include /**/ "ace/post.h"
#endif /* TAO_AV_ENDPOINT_STRATEGY_H */
