/* -*- C++ -*- */

// $Id$
// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   Protocol_Factory
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_PROTOCOL_FACTORY_T_H
#define TAO_AV_PROTOCOL_FACTORY_T_H
#include "ace/pre.h"

#include "ace/Service_Object.h"
#include "Policy.h"
#include "FlowSpec_Entry.h"

class TAO_AV_Protocol_Object
{
public:
  TAO_AV_Protocol_Object (void);

  TAO_AV_Protocol_Object (TAO_AV_Callback *callback,
                          TAO_AV_Transport *transport);
  // constructor.

  virtual ~TAO_AV_Protocol_Object (void);
  // Destructor

  virtual int open (TAO_AV_Callback *callback,
                    TAO_AV_Transport *transport);

  virtual int handle_input (void) = 0;

  virtual int handle_control_input (ACE_Message_Block *control_frame,
                                    const ACE_Addr &peer_address);
  // Called on a control object.

  virtual int set_policies (const TAO_AV_PolicyList &policy_list);
  virtual TAO_AV_PolicyList get_policies (void);
  // set/get policies.

  virtual int start (void);
  virtual int stop (void);
  // start/stop the flow.

  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0) = 0;
  // send a data frame.

  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0) = 0;
  // send a frame in iovecs.

  virtual int send_frame (const char *buf,
                          size_t len) = 0;

  virtual void control_object (TAO_AV_Protocol_Object *object);
  virtual int destroy (void) = 0;
  // end the stream.
  TAO_AV_Transport *transport (void);
protected:
  TAO_AV_Transport *transport_;
  TAO_AV_PolicyList policy_list_;
  TAO_AV_Callback *callback_;
};

class TAO_AV_Export TAO_AV_Flow_Protocol_Factory : public ACE_Service_Object
{
public:
  TAO_AV_Flow_Protocol_Factory (void);
  virtual ~TAO_AV_Flow_Protocol_Factory (void);
  virtual int init (int argc, char *argv[]);
  // Initialization hook.
  virtual int match_protocol (const char *flow_string);
  virtual TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                        TAO_Base_StreamEndPoint *endpoint,
                                                        TAO_AV_Flow_Handler *handler,
                                                        TAO_AV_Transport *transport);
  virtual const char *control_flow_factory (void);
};

#include "ace/post.h"
#endif /* TAO_AV_PROTOCOL_FACTORY_T_H */
