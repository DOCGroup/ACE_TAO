//$Id$

#include "Nil.h"

//------------------------------------------------------------
// TAO_AV_TCP_Object
//------------------------------------------------------------

int
TAO_AV_TCP_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 timestamp)
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

int
TAO_AV_TCP_Object::end_stream (void)
{
  return -1;
}

//------------------------------------------------------------
// TAO_AV_UDP_Object
//------------------------------------------------------------

int
TAO_AV_UDP_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 timestamp)
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


int
TAO_AV_UDP_Object::end_stream (void)
{
  return -1;
}

//------------------------------------------------------------
// TAO_AV_UDP_MCast_Object
//------------------------------------------------------------

int
TAO_AV_UDP_MCast_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 timestamp)
{
  int result = this->transport_->send (frame);
  if (result < 0)
    return result;
  return 0;
}

TAO_AV_UDP_MCast_Object::TAO_AV_UDP_MCast_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
}

int
TAO_AV_UDP_MCast_Object::end_stream (void)
{
  return -1;
}


