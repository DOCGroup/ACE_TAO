/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Endpoint_Strategy.h
//
// = AUTHOR
//    Sumedh Mungee <sumedh@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_ENDPOINT_STRATEGY_H
#define TAO_AV_ENDPOINT_STRATEGY_H

#include "orbsvcs/AV/AVStreams_i.h"
#include "tao/TAO.h"

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Base class to define various endpoint strategies
//    used by the MMDevice to create the Endpoint and Vdev
{
public:
  TAO_AV_Endpoint_Strategy (void);
  // Constructor

  virtual ~TAO_AV_Endpoint_Strategy (void);
  // Destructor

  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an A type endpoint

  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Called by the MMDevice, when it needs to create an B type endpoint

 protected:
  AVStreams::StreamEndPoint_A_ptr stream_endpoint_a_;
  // The "A" stream endpoint

  AVStreams::StreamEndPoint_B_ptr stream_endpoint_b_;
  // The "B" stream endpoint

  AVStreams::VDev_ptr vdev_;
  // The vdev

};

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy
  : public TAO_AV_Endpoint_Strategy
// = DESCRIPTION
//    Process-based strategy for creating endpoints
//    Abstract base class.
{
 public:
  TAO_AV_Endpoint_Process_Strategy (ACE_Process_Options *process_options);
  // Constructor. The process_options contain the name and arguments
  // for the process to be created

  virtual ~TAO_AV_Endpoint_Process_Strategy (void);
  // Destructor.

  virtual int activate (void);
  // creates a new child process, and waits on a semaphore
  // until the child process has finished creating the endpoints

 protected:
  virtual int bind_to_naming_service (CORBA::Environment &env);
  // Bind to the naming service

  virtual int get_stream_endpoint (CORBA::Environment &env) = 0;
  // Get the object reference for the newly created stream
  // endpoint (which will be in the child process)
  // Subclasses will define the functionality for this

  virtual int get_vdev (CORBA::Environment &env);
  // Get the Vdev object reference for the newly created
  // endpoint

  CosNaming::NamingContext_var naming_context_;
  // Naming context

  ACE_Process_Options *process_options_;
  // Name and arguments for the process to be created

  char host_[MAXHOSTNAMELEN];
  // name of this host used for resolving unique names.

  pid_t pid_;
  // My child's process id.
};


// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_A
  : public TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy to create "A" type endpoints
{
 public:
  TAO_AV_Endpoint_Process_Strategy_A (ACE_Process_Options *process_options);
  // Constructor

  virtual ~TAO_AV_Endpoint_Process_Strategy_A (void);
  // Destructor.

 protected:
  virtual int create_A (AVStreams::StreamEndPoint_A_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Creates an "A" type stream endpoint, and a vdev

  virtual int get_stream_endpoint (CORBA::Environment &env);
  // Gets the "A" type stream endpoint from the child process

};

// ----------------------------------------------------------------------

class TAO_ORBSVCS_Export TAO_AV_Endpoint_Process_Strategy_B
  : public TAO_AV_Endpoint_Process_Strategy
// = DESCRIPTION
//    Process-based strategy to create "B" type endpoints
{
 public:
  TAO_AV_Endpoint_Process_Strategy_B (ACE_Process_Options *process_options);
  // Constructor

  virtual ~TAO_AV_Endpoint_Process_Strategy_B (void);
  // Destructor.

 protected:
  virtual int create_B (AVStreams::StreamEndPoint_B_ptr &stream_endpoint,
                        AVStreams::VDev_ptr &vdev,
                        CORBA::Environment &env);
  // Creates a "B" type stream endpoint, and a vdev


  virtual int get_stream_endpoint (CORBA::Environment &env);
  // Gets the object reference of the "B" type streamendpoint.

};

// Include the templates here.
#include "Endpoint_Strategy_T.h"

#endif /* TAO_AV_ENDPOINT_STRATEGY_H */
