/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    server.h
//
// = DESCRIPTION 
//    Test server for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#if !defined (AVSTREAMS_SERVER_H)
#define AVSTREAMS_SERVER_H

#include "orbsvcs/AV/AVStreams_i.h"
#include "tao/TAO.h"

class Video_Server_StreamEndPoint : public virtual TAO_Server_StreamEndPoint
{
  // = TITLE
  //    Server Stream Endpoint for A/V Streams demo.
  //
  // = DESCRIPTION
  //    Encapsulates server side transport functionality.
public:
  Video_Server_StreamEndPoint (void);
  // Constructor.
  
  virtual void handle_stop (const AVStreams::flowSpec &the_spec,
                            CORBA::Environment &env);
  // Handles stream stoppage.
  
  virtual void handle_start (const AVStreams::flowSpec &the_spec,  
                             CORBA::Environment &env);
  // Handles stream start. Starts the flow of data.
  
  virtual void handle_destroy (const AVStreams::flowSpec &the_spec,  
                               CORBA::Environment &env);
  // Handles the destruction of the stream, tears down the transport.

  virtual CORBA::Boolean handle_connection_requested (AVStreams::StreamEndPoint_ptr initiator, 
                                                      CORBA::Boolean is_mcast, 
                                                      AVStreams::streamQoS &qos, 
                                                      AVStreams::flowSpec &the_spec,  
                                                      CORBA::Environment &env);
  // Handles stream bind, establishes the transport.
};

#endif /* AVSTREAMS_SERVER_H */
