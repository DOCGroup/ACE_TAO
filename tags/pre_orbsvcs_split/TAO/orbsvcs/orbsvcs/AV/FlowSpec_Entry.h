/* -*- C++ -*- */

//$Id$

// ============================================================================
//
// = LIBRARY
//   ORBSVCS (AVStreams)
//
// = FILENAME
//   FlowSpec_Entry.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_FLOWSPEC_ENTRY_H
#include "ace/pre.h"
#define TAO_AV_FLOWSPEC_ENTRY_H

#include "orbsvcs/AV/AV_export.h"
#include "AV_Core.h"
#include "ace/Addr.h"

class TAO_AV_Export TAO_Tokenizer
{
public:
  TAO_Tokenizer (const char *string,char delimiter);
  // constructor.

  ~TAO_Tokenizer ();
  // destructor.

  int parse (const char *string,char delimiter);
  // parses the string and tokenizes it.

  char *token (void);
  // Returns the next token.

  int num_tokens (void);
  // Number of tokens.

  char *operator [] (size_t index) const;

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

class TAO_AV_Export TAO_FlowSpec_Entry
{
public:
  // = TITLE
  //     An helper entry class in the flow spec sequence passed to bind_devs.
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

  TAO_FlowSpec_Entry (void);
  // default constructor.

  TAO_FlowSpec_Entry (const char *flowname,
                      const char *direction,
                      const char *format_name,
                      const char *flow_protocol,
                      const char *carrier_protocol,
                      ACE_Addr *address);
  // constructor to construct an entry from the arguments.

  TAO_FlowSpec_Entry (const char *flowname,
                      const char *direction,
                      const char *format_name,
                      const char *flow_protocol,
                      const char *address);

  virtual int parse (const char* flowSpec_entry) = 0;
  // construct the entry from a string specified by the flowSpec grammar.

  virtual ~TAO_FlowSpec_Entry (void);
  // virtual destructor.

  int direction (void);
  // accessor to the direction.

  virtual Role role (void) = 0;
  void role (Role role);
  char * direction_str (void);
  // string version of direction .

  char *flow_protocol_str (void);
  // accesor to the flow protocol string.

  void flow_protocol_str (const char *flow_protocol_str);
  // set the flow protocol string.

  ACE_Addr *address (void);
  // Address of the carrier protocol.

  char * address_str (void);
  // Address in string format i. hostname:port.

  TAO_AV_Core::Protocol carrier_protocol (void);
  // carrier protocol i.e TCP,UDP,RTP/UDP.

  char * carrier_protocol_str (void);
  // string version of carrier protocol.

  char *format (void);
  // format to be used for this flow.

  const char *flowname (void);
  // name of this flow.

  virtual char *entry_to_string (void) = 0;
  // converts the entry to a string.

  int set_peer_addr (ACE_Addr *peer_addr);
  ACE_Addr *get_peer_addr (void);
  int set_local_addr (ACE_Addr *peer_addr);
  ACE_Addr *get_local_addr (void);
  char *get_local_addr_str (void);
  TAO_AV_Transport *transport (void);
  void transport (TAO_AV_Transport *transport);
  TAO_AV_Flow_Handler* handler (void);
  void handler (TAO_AV_Flow_Handler *handler);
  TAO_AV_Protocol_Object* protocol_object (void);
  void protocol_object (TAO_AV_Protocol_Object *object);

  int parse_address (char *format_string);
  // sets the address for this flow.

  int is_multicast (void);
  // returns true for a multicast address.

protected:

  int parse_flow_protocol_string (char *flow_options_string);
  // parses the flow protocol string with tokens separated by :

  int set_direction (char *direction_string);
  // sets the direction flag.


  int set_protocol (void);
  // sets the protocol_ enum from the carrier_protocol_ string.

  ACE_Addr *address_;
  // Addr information for the carrier protocol.

  char *address_str_;
  // Addr in string format i.e hostname:port.

  char *format_;
  // format string.

  Direction direction_;
  // Direction of this flow.

  char *direction_str_;
  // string representation of the direction.

  char *flowname_;
  // name of this flow.

  TAO_AV_Core::Protocol protocol_;
  // name of the protocol used.

  char *carrier_protocol_;
  // carrier protocol string.

  char *flow_protocol_;
  // flow protocol string.

  int use_flow_protocol_;
  char *entry_;
  // The flowspec entry;

  int is_multicast_;
  ACE_Addr *peer_addr_;
  ACE_Addr *local_addr_;
  TAO_AV_Transport *transport_;
  TAO_AV_Flow_Handler *handler_;
  TAO_AV_Protocol_Object *protocol_object_;
  Role role_;
};

class TAO_AV_Export TAO_Forward_FlowSpec_Entry
  :public TAO_FlowSpec_Entry
{
public:
  enum Position {TAO_AV_FLOWNAME = 0,
                 TAO_AV_DIRECTION = 1,
                 TAO_AV_FORMAT = 2,
                 TAO_AV_FLOW_PROTOCOL = 3,
                 TAO_AV_ADDRESS = 4};

  TAO_Forward_FlowSpec_Entry (void);
  // default constructor.

  TAO_Forward_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *carrier_protocol,
                              ACE_Addr *address);
  // constructor to construct an entry from the arguments.

  TAO_Forward_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *address);

  virtual char *entry_to_string (void);
  // converts the entry to a string.

  virtual Role role (void);
  virtual int parse (const char* flowSpec_entry);
  // construct the entry from a string specified by the flowSpec grammar.
};

class TAO_AV_Export TAO_Reverse_FlowSpec_Entry
  :public TAO_FlowSpec_Entry
{
public:
  enum Position {TAO_AV_FLOWNAME = 0,
                 TAO_AV_ADDRESS = 1,
                 TAO_AV_FLOW_PROTOCOL = 2,
                 TAO_AV_DIRECTION = 3,
                 TAO_AV_FORMAT = 4};

  TAO_Reverse_FlowSpec_Entry (void);
  // default constructor.

  TAO_Reverse_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *carrier_protocol,
                              ACE_Addr *address);
  // constructor to construct an entry from the arguments.

  TAO_Reverse_FlowSpec_Entry (const char *flowname,
                              const char *direction,
                              const char *format_name,
                              const char *flow_protocol,
                              const char *address);
  // Takes the address in protocol=endpoint form.

  virtual char *entry_to_string (void);
  // converts the entry to a string.

  virtual Role role (void);
  virtual int parse (const char* flowSpec_entry);
  // construct the entry from a string specified by the flowSpec grammar.
};

#include "Transport.h"

#if defined (__ACE_INLINE__)
#include "FlowSpec_Entry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_AV_FLOWSPEC_ENTRY_H */
