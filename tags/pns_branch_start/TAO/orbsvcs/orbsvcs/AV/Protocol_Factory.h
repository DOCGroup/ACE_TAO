/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Protocol_Factory.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_PROTOCOL_FACTORY_T_H
#define TAO_AV_PROTOCOL_FACTORY_T_H
#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"
#include "Policy.h"
#include "FlowSpec_Entry.h"

/**
 * @class TAO_AV_Protocol_Object
 * @brief
 */
class TAO_AV_Export TAO_AV_Protocol_Object
{
public:
  TAO_AV_Protocol_Object (void);

  /// constructor.
  TAO_AV_Protocol_Object (TAO_AV_Callback *callback,
                          TAO_AV_Transport *transport);

  /// Destructor
  virtual ~TAO_AV_Protocol_Object (void);

  virtual int open (TAO_AV_Callback *callback,
                    TAO_AV_Transport *transport);

  virtual int handle_input (void) = 0;

  /// Called on a control object.
  virtual int handle_control_input (ACE_Message_Block *control_frame,
                                    const ACE_Addr &peer_address);

  /// set/get policies.
  virtual int set_policies (const TAO_AV_PolicyList &policy_list);
  virtual TAO_AV_PolicyList get_policies (void);

  /// start/stop the flow.
  virtual int start (void);
  virtual int stop (void);

  /// send a data frame.
  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0) = 0;

  /// send a frame in iovecs.
  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0) = 0;

  virtual int send_frame (const char *buf,
                          size_t len) = 0;

  /// end the stream.
  virtual void control_object (TAO_AV_Protocol_Object *object);
  virtual int destroy (void) = 0;
  TAO_AV_Transport *transport (void);
protected:
  TAO_AV_Transport *transport_;
  TAO_AV_PolicyList policy_list_;
  TAO_AV_Callback *callback_;
};

/**
 * @class TAO_AV_Flow_Protocol_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_Flow_Protocol_Factory : public ACE_Service_Object
{
public:
  /// Initialization hook.
  TAO_AV_Flow_Protocol_Factory (void);
  virtual ~TAO_AV_Flow_Protocol_Factory (void);
  virtual int init (int argc, char *argv[]);
  virtual int match_protocol (const char *flow_string);
  virtual TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                        TAO_Base_StreamEndPoint *endpoint,
                                                        TAO_AV_Flow_Handler *handler,
                                                        TAO_AV_Transport *transport);
  virtual const char *control_flow_factory (void);
  int ref_count;
};

#include /**/ "ace/post.h"
#endif /* TAO_AV_PROTOCOL_FACTORY_T_H */
