 /**
  * @file PIP_Messages
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a heirarchy of
  * classes that represent the various messages used in the
  * priority inheritance protocol
*/

#ifndef _PIP_MESSAGES_H_
#define _PIP_MESSAGES_H_

#include "ace/Message_Block.h"
#include "ace/SOCK_Stream.h"
#include "ace/Vector_T.h"

/**
 * @class ACE_PIP_Message
 * @brief Base class for all messages used in
 * the implementation of a distributed priority inheritance
 * protocol.
 * 
 * Base class for all messages used in the implementation of a distributed
 * priority inheritance protocol. Provides an interface for message (de)serialization,
 * message chaining, packing, unpacking, and payload ownership transfer
 */
class ACE_Export ACE_PIP_Message
{
 public:
  
  ACE_PIP_Message(); 
  virtual ~ACE_PIP_Message();
  
  /// Send the contents of this message over the stream.
  virtual int serialize(ACE_SOCK_Stream& stream) = 0;
  
  /// Get the next message struct.
  virtual ACE_PIP_Message* next();
  
  /// Set the next message struct.
   virtual void next(ACE_PIP_Message* next);
   
   /// Returns the next message, making the caller
   /// the new owner.
   virtual ACE_PIP_Message* release_next();
   
   /// Get the message block.
   virtual ACE_Message_Block* block();
   
   /// Set the message block and populate the message struct
   /// with message contents.
   virtual void block(ACE_Message_Block* block);
   
   /// Get the message block, making the caller the new owner.
   virtual ACE_Message_Block* release_block();
   
   /// Place the values in the message struct into the message block.
   virtual int pack() = 0;
   
   /// Populate the message struct using values from the message block.
   virtual void unpack() = 0;
   
   /// This is temporarily public to facilitate testing.
   /// It should eventually be made private.
   ACE_Message_Block* block_;

  protected:
   
   // Indicates values in structure are newer than values in the
   // message block.
   bool dirty_;
   
   ACE_PIP_Message* next_;	
};

/**
 * @class ACE_PIP_Data_Message
 * @brief Structure representing the fields of an application-
 * level protocol message and associated header values
 * 
 * Structure representing the fields of an appliation level
 * protocol message and associated header values. Structure is that
 * of several contiguous ACE_Message_Block's. The message is configurable
 * to support any application-level protocol that contains at least the following
 * data: source address, destination address, reply expectation, and priority
 * 
*/
class ACE_Export ACE_PIP_Data_Message : public ACE_PIP_Message
{
  public:
   
   ACE_PIP_Data_Message();
   virtual ~ACE_PIP_Data_Message(){}
   
   /// Send the contents of this message over the stream.
   virtual int serialize(ACE_SOCK_Stream& stream);
   
   /// Determine if a reply message is expected
   bool reply_expected() const;
   void reply_expected(bool expected);
   
   /// Determine the priority at which this message should be handled
   ACE_UINT32 message_priority() const;
   void message_priority(ACE_UINT32 priority);

   /// Determine the ID of the destination handler
   ACE_UINT32 destination_handler_ID() const;
   void destination_handler_ID(ACE_UINT32 ID);

   /// Determine the ID of the sending handler
   ACE_UINT32 source_handler_ID() const;
   void source_handler_ID(ACE_UINT32 ID);

   /// Determine the ID of the destination site
   ACE_UINT32 destination_site_ID() const;
   void destination_site_ID(ACE_UINT32 ID);
   
   /// Determine the ID of the sending site
   ACE_UINT32 source_site_ID() const;
   void source_site_ID(ACE_UINT32 ID);

   // Place the values from the struct into the message blocks.
   virtual int pack();
   
   // Extract the values from the message blocks into the structs.
   virtual void unpack();

 private:
   
   ACE_UINT32 message_priority_;
   bool       reply_expected_;
   ACE_UINT32 source_handler_id_;
   ACE_UINT32 source_site_id_;
   ACE_UINT32 destination_handler_id_;
   ACE_UINT32 destination_site_id_;
};

/**
 * @class ACE_PIP_Protocol_Message
 * @brief Structure representing a message supported by the priority
 * inheritance protocol
 * 
*/

class ACE_Export ACE_PIP_Accel_Message : public ACE_PIP_Message
{	
  public:
   
   ACE_PIP_Accel_Message();
   virtual ~ACE_PIP_Accel_Message(){}
   
   /// Send the contents of this message over the stream.
   virtual int serialize(ACE_SOCK_Stream& stream);
   
   ACE_UINT32 old_priority() const;
   void       old_priority(ACE_UINT32 priority);
   
   ACE_UINT32 new_priority() const;
   void       new_priority(ACE_UINT32 priority);
   
   /// Get the address of the application receiving the message.
   ACE_UINT32 destination_address() const;
   void destination_address(const ACE_UINT32& address);
   
   u_short destination_port() const;
   void destination_port(u_short port);

   /// Place the values in the message struct into the message block.
   virtual int pack();
   
   /// Extract the values from the message block and store them in the struct.
   virtual void unpack();

   /// Return a copy of the this message
   ACE_PIP_Accel_Message* copy();
   
  private:
   
   const ACE_UINT32 ACCEL_HEADER_LENGTH_;
   ACE_UINT32   destination_address_;
   u_short      destination_port_;
   ACE_UINT32   new_priority_;
   ACE_UINT32   old_priority_;
};

/**
 * @class ACE_PIP_Accel_Message
 * @brief Structure representing an acceleration message
 * used in the implementation of a priority inheritance protocol
 * 
 * Structure representing an acceleration message used in the 
 * implementation of a priority inheritance protocol. Indicates the 
 * old and new priority of the targeted process, as well as the address
 * of handler to which the associated message was sent. 
*/
class ACE_Export ACE_PIP_Protocol_Message : public ACE_PIP_Message
{
  public:
   
   enum Message_Type { NONE, ACCEL, DATA, REQUEST, RESPONSE };
   
   ACE_PIP_Protocol_Message();
   virtual ~ACE_PIP_Protocol_Message(){}
   
   /// Send the contents of this message over the stream.
   virtual int serialize(ACE_SOCK_Stream& stream);
   
   /// Receive the contents of this message from the stream.
   virtual int deserialize(ACE_SOCK_Stream& stream);

   /// Set the next message in the chain.
   virtual void next(ACE_PIP_Message* next);

   /// Get the next message in the chain
   virtual ACE_PIP_Message* next();
   
   /// Determine the type of message this header has been tacked onto.
   Message_Type message_type() const;
   void         message_type(Message_Type type);
   
   /// Determine which call chain this message is associated with.
   ACE_UINT64 message_id() const;
   void       message_id(ACE_UINT64 id);
   
   /// Attach message block as payload of priority inheritance
   /// protocol message.
   int process_message_payload(ACE_Message_Block* payload);
   
   virtual int pack();
   virtual void unpack();

   /// Make a copy of the header of this message, i.e. without
   /// data or accel payload
   ACE_PIP_Protocol_Message* copy();
   
   const int FIXED_HEADER_LENGTH_;
   
  private:
   
   Message_Type   message_type_;
   ACE_UINT32     num_payload_blocks_;
   ACE_Vector<ACE_UINT32> payload_block_lengths_;
   ACE_UINT64     message_id_;
};

#if defined (__ACE_INLINE__)
#include "PIP_Messages.inl"
#endif /* __ACE_INLINE__ */


#endif

