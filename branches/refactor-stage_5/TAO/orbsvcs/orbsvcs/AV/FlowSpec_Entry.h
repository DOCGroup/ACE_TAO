/* -*- C++ -*- */


// ============================================================================
/**
 *  @file    FlowSpec_Entry.h
 *
 *  $Id$
 *
 *  @author  Nagarajan Surendran <naga@cs.wustl.edu>
 *
 */
// ============================================================================

#ifndef TAO_AV_FLOWSPEC_ENTRY_H
#define TAO_AV_FLOWSPEC_ENTRY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/AV/AV_export.h"
#include "AV_Core.h"
#include "ace/Addr.h"
#include "ace/Containers.h"

/**
 * @class TAO_Tokenizer
 * @brief
 */
class TAO_AV_Export TAO_Tokenizer
{
public:
  /// constructor.
  TAO_Tokenizer (const char *string,char delimiter);

  /// destructor.
  ~TAO_Tokenizer ();

  /// parses the string and tokenizes it.
  int parse (const char *string,char delimiter);

  /// Returns the next token.
  char *token (void);

  /// Number of tokens.
  int num_tokens (void);

  const char *operator [] (size_t index) const;

protected:
  ACE_Array<char*> token_array_;
  size_t count_;
  size_t num_tokens_;
  char string_ [BUFSIZ];
};

// Forward declaration.
class TAO_AV_Transport;
class TAO_AV_Flow_Handler;
class TAO_AV_Protocol_Object;

/**
 * @class TAO_FlowSpec_Entry
 * @brief  A helper entry class in the flow spec sequence passed to
 *         bind_devs.
 */
class TAO_AV_Export TAO_FlowSpec_Entry
{
public:

  enum Direction
  {
    TAO_AV_INVALID   = -1,
    TAO_AV_DIR_IN    =  0,
    TAO_AV_DIR_OUT   =  1
  };

  enum Role
  {
    TAO_AV_INVALID_ROLE = -1,
    TAO_AV_PRODUCER = 0,
    TAO_AV_CONSUMER = 1
  };

  /// default constructor.
  TAO_FlowSpec_Entry (void);

  /// constructor to construct an entry from the arguments.
  TAO_FlowSpec_Entry (const char *flowname,
                      const char *direction,
                      const char *format_name,
                      const char *flow_protocol,
                      const char *carrier_protocol,
                      ACE_Addr *fwd_address,
                      //ACE_Addr *peer_address,
                      ACE_Addr *control_address = 0);

  TAO_FlowSpec_Entry (const char *flowname,
                      const char *direction,
                      const char *format_name,
                      const char *flow_protocol,
                      const char *fwd_address);
                      //const char *peer_address);

  /// construct the entry from a string specified by the flowSpec grammar.
  virtual int parse (const char* flowSpec_entry) = 0;

  /// virtual destructor.
  virtual ~TAO_FlowSpec_Entry (void);

  /// accessor to the direction.
  int direction (void);

  virtual Role role (void) = 0;
  void role (Role role);
  /// accessor to string version of direction .
  const char * direction_str (void) const;

  /// accessor to the flow protocol string.
  const char *flow_protocol_str (void) const;

  /// set the flow protocol string.
  void flow_protocol_str (const char *flow_protocol_str);

  /// accessor to address of the carrier protocol.
  //ACE_Addr *fwd_address (void);
  ACE_Addr *address (void);
  ACE_Addr *control_address (void);
  void address (ACE_Addr *address);
  void control_address (ACE_Addr *address);

  /// Address in string format i. hostname:port.
  const char *address_str (void) const;
  //  const char * peer_address_str (void) const;

  /// accessor to carrier protocol i.e TCP,UDP,RTP/UDP.
  TAO_AV_Core::Protocol carrier_protocol (void);

  /// accessor to string version of carrier protocol.
  const char * carrier_protocol_str (void) const;

  /// accessor to format to be used for this flow.
  const char *format (void) const;

  /// accessor to name of this flow.
  const char *flowname (void) const;

  /// converts the entry to a string.
  virtual const char *entry_to_string (void) = 0;

  int set_peer_addr (ACE_Addr *peer_addr);
  ACE_Addr *get_peer_addr (void);
  int set_peer_control_addr (ACE_Addr *peer_control_addr);
  ACE_Addr *get_peer_control_addr (void);

  int set_local_addr (ACE_Addr *local_addr);
  ACE_Addr *get_local_addr (void);
  char *get_local_addr_str (void);
  int set_local_control_addr (ACE_Addr *local_control_addr);
  ACE_Addr *get_local_control_addr (void);
  char *get_local_control_addr_str (void);

  TAO_AV_Transport *transport (void);
  void transport (TAO_AV_Transport *transport);
  TAO_AV_Transport *control_transport (void);
  void control_transport (TAO_AV_Transport *control_transport);

  TAO_AV_Flow_Handler* handler (void);
  void handler (TAO_AV_Flow_Handler *handler);
  TAO_AV_Flow_Handler* control_handler (void);
  void control_handler (TAO_AV_Flow_Handler *control_handler);

  TAO_AV_Protocol_Object* protocol_object (void);
  void protocol_object (TAO_AV_Protocol_Object *object);
  TAO_AV_Protocol_Object* control_protocol_object (void);
  void control_protocol_object (TAO_AV_Protocol_Object *object);

  /// sets the address for this flow.
  int parse_address (const char *format_string,
                     TAO_AV_Core::Flow_Component flow_component);

  /// returns true for a multicast address.
  int is_multicast (void);

protected:

  /// parses the flow protocol string with tokens separated by :
  int parse_flow_protocol_string (const char *flow_options_string);

  /// sets the direction flag.
  int set_direction (const char *direction_string);

  /// sets the protocol_ enum from the carrier_protocol_ string.
  int set_protocol (void);

  /// Addr information for the carrier protocol.
  ACE_Addr *address_;
  int clean_up_address_;         // added to clean up a memory leak
  ACE_Addr *control_address_;
  int clean_up_control_address_; // added to clean up a memory leak

  /// Fwd Addr in string format i.e hostname:port.
  ACE_CString address_str_;

  /// Peer Addr in string format i.e hostname:port.
  ACE_CString peer_address_str_;

  /// format string.
  ACE_CString format_;

  /// Direction of this flow.
  Direction direction_;

  /// string representation of the direction.
  ACE_CString direction_str_;

  /// name of this flow.
  ACE_CString flowname_;

  /// name of the protocol used.
  TAO_AV_Core::Protocol protocol_;

  /// carrier protocol string.
  ACE_CString carrier_protocol_;

  /// flow protocol string.
  ACE_CString flow_protocol_;

  int use_flow_protocol_;

  /// The flowspec entry;
  ACE_CString entry_;

  int is_multicast_;
  ACE_Addr *peer_addr_;
  ACE_Addr *peer_control_addr_;
  ACE_Addr *local_addr_;
  ACE_Addr *local_control_addr_;
  TAO_AV_Transport *transport_;
  TAO_AV_Transport *control_transport_;
  TAO_AV_Flow_Handler *handler_;
  TAO_AV_Flow_Handler *control_handler_;
  TAO_AV_Protocol_Object *protocol_object_;
  TAO_AV_Protocol_Object *control_protocol_object_;
  Role role_;
};


/**
 * @class TAO_Forward_FlowSpec_Entry
 * @brief
 */
class TAO_AV_Export TAO_Forward_FlowSpec_Entry
  :public TAO_FlowSpec_Entry
{
public:
  enum Position {TAO_AV_FLOWNAME = 0,
                 TAO_AV_DIRECTION = 1,
                 TAO_AV_FORMAT = 2,
                 TAO_AV_FLOW_PROTOCOL = 3,
                 TAO_AV_ADDRESS = 4,
		 TAO_AV_PEER_ADDR = 5};

  /// default constructor.
  TAO_Forward_FlowSpec_Entry (void);

  /// constructor to construct an entry from the arguments.
  TAO_Forward_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *carrier_protocol,
                              ACE_Addr *address,
                              ACE_Addr *control_address = 0);

  TAO_Forward_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *address);

  virtual ~TAO_Forward_FlowSpec_Entry (void);

  /// converts the entry to a string.
  virtual const char *entry_to_string (void);

  virtual Role role (void);

  /// construct the entry from a string specified by the flowSpec grammar.
  virtual int parse (const char* flowSpec_entry);
};

/**
 * @class TAO_Reverse_FlowSpec_Entry
 * @brief
 */
class TAO_AV_Export TAO_Reverse_FlowSpec_Entry
  :public TAO_FlowSpec_Entry
{
public:
  enum Position {TAO_AV_FLOWNAME = 0,
                 TAO_AV_ADDRESS = 1,
                 TAO_AV_FLOW_PROTOCOL = 2,
                 TAO_AV_DIRECTION = 3,
                 TAO_AV_FORMAT = 4};

  // default constructor.
  TAO_Reverse_FlowSpec_Entry (void);

  // constructor to construct an entry from the arguments.
  TAO_Reverse_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *carrier_protocol,
                              ACE_Addr *address,
                              ACE_Addr *control_address = 0);

  // Takes the address in protocol=endpoint form.
  TAO_Reverse_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *address);

  virtual ~TAO_Reverse_FlowSpec_Entry (void);

  /// converts the entry to a string.
  virtual const char *entry_to_string (void);

  virtual Role role (void);

  /// construct the entry from a string specified by the flowSpec grammar.
  virtual int parse (const char* flowSpec_entry);
};

#include "Transport.h"

#if defined (__ACE_INLINE__)
#include "FlowSpec_Entry.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_AV_FLOWSPEC_ENTRY_H */
