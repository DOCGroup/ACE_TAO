/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   Nil.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_NIL_H
#define TAO_AV_NIL_H

#include "Transport.h"
#include "Policy.h"

class TAO_ORBSVCS_Export TAO_AV_TCP_Object
  :public TAO_AV_Protocol_Object
{
public:
  TAO_AV_TCP_Object (TAO_AV_Callback *callback,
                     TAO_AV_Transport *transport = 0);

  virtual ~TAO_AV_TCP_Object (void);
  // Dtor
  
  virtual int send_frame (ACE_Message_Block *frame,
                          ACE_UINT32 timestamp = 0);
  // send a data frame.

  virtual int end_stream (void);
  // end the stream.

};

class TAO_ORBSVCS_Export TAO_AV_UDP_Object
  :public TAO_AV_Protocol_Object
{
public:
  TAO_AV_UDP_Object (TAO_AV_Callback *callback,
                     TAO_AV_Transport *transport = 0);

  virtual ~TAO_AV_UDP_Object (void);
  // Dtor

  virtual int send_frame (ACE_Message_Block *frame,
                          ACE_UINT32 timestamp = 0);
  // send a data frame.

  virtual int end_stream (void);
  // end the stream.
};

class TAO_ORBSVCS_Export TAO_AV_UDP_MCast_Object
  :public TAO_AV_Protocol_Object
{
public:
  TAO_AV_UDP_MCast_Object (TAO_AV_Callback *callback,
                           TAO_AV_Transport *transport = 0);

  virtual ~TAO_AV_UDP_MCast_Object (void);
  // Dtor

  virtual int send_frame (ACE_Message_Block *frame,
                          ACE_UINT32 timestamp = 0);
  // send a data frame.

  virtual int end_stream (void);
  // end the stream.
};
#endif /* TAO_AV_NIL_H */
