//$Id$

#include "Nil.h"

//------------------------------------------------------------
// TAO_AV_TCP_Object
//------------------------------------------------------------

int
TAO_AV_TCP_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 /*timestamp*/)
{
  int result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

TAO_AV_TCP_Object::TAO_AV_TCP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
}

TAO_AV_TCP_Object::~TAO_AV_TCP_Object (void)
{
  // No-op
}
int
TAO_AV_TCP_Object::end_stream (void)
{
  this->callback_->handle_end_stream ();
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_Object
//------------------------------------------------------------

int
TAO_AV_UDP_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 /*timestamp*/)
{
  ACE_DEBUG ((LM_DEBUG,"TAO_AV_UDP_Object::send_frame\n"));
  int result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

TAO_AV_UDP_Object::TAO_AV_UDP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
}

TAO_AV_UDP_Object::~TAO_AV_UDP_Object (void)
{
  //no-op
}

int
TAO_AV_UDP_Object::end_stream (void)
{
  this->callback_->handle_end_stream ();
  return 0;
}

//------------------------------------------------------------
// TAO_AV_UDP_MCast_Object
//------------------------------------------------------------


TAO_AV_UDP_MCast_Object::TAO_AV_UDP_MCast_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
}

TAO_AV_UDP_MCast_Object::~TAO_AV_UDP_MCast_Object (void)
{
  //no-op
}

int
TAO_AV_UDP_MCast_Object::send_frame (ACE_Message_Block *frame,
                                     ACE_UINT32 /*timestamp*/)
{
  int result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_UDP_MCast_Object::end_stream (void)
{
  this->callback_->handle_end_stream ();
  return 0;
}


