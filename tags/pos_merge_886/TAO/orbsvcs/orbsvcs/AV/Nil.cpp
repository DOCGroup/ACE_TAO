//$Id$

#include "Nil.h"
#include "FlowSpec_Entry.h"

// //------------------------------------------------------------
// // TAO_AV_UDP_MCast_Object
// //------------------------------------------------------------


// TAO_AV_UDP_MCast_Object::TAO_AV_UDP_MCast_Object (TAO_AV_Callback *callback,
//                                                   TAO_AV_Transport *transport)
//   :TAO_AV_Protocol_Object (callback,transport)
// {
// }

// TAO_AV_UDP_MCast_Object::~TAO_AV_UDP_MCast_Object (void)
// {
//   //no-op
// }

// int
// TAO_AV_UDP_MCast_Object::send_frame (ACE_Message_Block *frame,
//                                      TAO_AV_frame_info *frame_info)
// {
//   int result = this->transport_->send (frame);
//   if (result < 0)
//     return result;
//   return 0;
// }

// int 
// TAO_AV_UDP_MCast_Object::send_frame (const iovec *iov,
//                                      int iovcnt,
//                                      TAO_AV_frame_info *frame_info)
// {
//   return this->transport_->send (iov,iovcnt);
// }

// int
// TAO_AV_UDP_MCast_Object::end_stream (void)
// {
//   this->callback_->handle_end_stream ();
//   return 0;
// }
