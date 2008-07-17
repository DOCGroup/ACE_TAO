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

#include <iostream>

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
  virtual ACE_PIP_Message* get_next();
  
  /// Set the next message struct.
   virtual void set_next(ACE_PIP_Message* next);
   
   /// Returns the next message, making the caller
   /// the new owner.
   virtual ACE_PIP_Message* release_next();
   
   /// Get the message block.
   virtual ACE_Message_Block* get_block();
   
   /// Set the message block and populate the message struct
   /// with message contents.
   virtual void set_block(ACE_Message_Block* block);
   
   /// Get the message block, making the caller the new owner.
   virtual ACE_Message_Block* release_block();
   
   /// Place the values in the message struct into the message block.
   virtual void pack() = 0;
   
   /// Populate the message struct using values from the message block.
   virtual void unpack() = 0;
   
   /// This is temporarily public to facilitate testing.
   /// It should eventually be made private.
   ACE_Message_Block* block_;

   /// Print the contents of this struct to stdout.
   virtual void print() const = 0;


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
   bool get_reply_expected() const;
   void set_reply_expected(bool expected);
   
   /// Determine the priority at which this message should be handled
   ACE_UINT32 get_message_priority() const;
   void set_message_priority(ACE_UINT32 priority);

   /// Determine the ID of the destination handler
   ACE_UINT32 get_destination_handler_ID() const;
   void set_destination_handler_ID(ACE_UINT32 ID);

   /// Determine the ID of the sending handler
   ACE_UINT32 get_source_handler_ID() const;
   void set_source_handler_ID(ACE_UINT32 ID);

   /// Determine the ID of the destination site
   ACE_UINT32 get_destination_site_ID() const;
   void set_destination_site_ID(ACE_UINT32 ID);
   
   /// Determine the ID of the sending site
   ACE_UINT32 get_source_site_ID() const;
   void set_source_site_ID(ACE_UINT32 ID);

   // Place the values from the struct into the message blocks.
   virtual void pack();
   
   // Extract the values from the message blocks into the structs.
   virtual void unpack();

   /// Print the contents of this struct to stdout.
   virtual void print() const;

 private:
   
   ACE_UINT32 message_priority_;
   bool       reply_expected_;
   ACE_UINT32 source_handler_ID_;
   ACE_UINT32 destination_handler_ID_;
   ACE_UINT32 source_site_ID_;
   ACE_UINT32 destination_site_ID_;
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
   
   ACE_UINT32 get_old_priority() const;
   void       set_old_priority(ACE_UINT32 priority);
   
   ACE_UINT32 get_new_priority() const;
   void       set_new_priority(ACE_UINT32 priority);
   
   /// Get the address of the application receiving the message.
   ACE_UINT32 get_destination_address() const;
   void set_destination_address(const ACE_UINT32& address);
   
   u_short get_destination_port() const;
   void set_destination_port(u_short port);

   /// Place the values in the message struct into the message block.
   virtual void pack();
   
   /// Extract the values from the message block and store them in the struct.
   virtual void unpack();

   /// Print the contents of this struct to stdout.
   virtual void print() const;

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
   virtual void set_next(ACE_PIP_Message* next);
   
   /// Determine the type of message this header has been tacked onto.
   Message_Type get_message_type() const;
   void         set_message_type(Message_Type type);
   
   /// Determine which call chain this message is associated with.
   ACE_UINT64 get_message_id() const;
   void       set_message_id(ACE_UINT64 id);
   
   /// Attach message block as payload of priority inheritance
   /// protocol message.
   void process_message_payload(ACE_Message_Block* payload);
   
   virtual void pack();
   virtual void unpack();

   /// Print the contents of this struct to stdout.
   virtual void print() const;

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


/**************************************************
 *
 *  ACE_PIP_Message - Inline Methods 
 *
 **************************************************/
inline ACE_PIP_Message* ACE_PIP_Message::get_next()
{
   return next_;
}

inline void ACE_PIP_Message::set_next(ACE_PIP_Message* message)
{
   next_ = message;
}

inline ACE_PIP_Message* ACE_PIP_Message::release_next()
{
   ACE_PIP_Message* temp = next_;
   next_ = 0;
   return temp;
}

inline ACE_Message_Block* ACE_PIP_Message::get_block() 
{
   return block_;
}

inline ACE_Message_Block* ACE_PIP_Message::release_block()
{
   ACE_Message_Block* temp_block = block_;
   block_ = 0;
   dirty_ = true;
   return temp_block;
}

/**************************************************
 *
 *  ACE_PIP_Data_Message - Inline Methods 
 *
 **************************************************/

inline bool ACE_PIP_Data_Message::get_reply_expected() const
{
   return reply_expected_;
}

inline void ACE_PIP_Data_Message::set_reply_expected(bool expected)
{
   dirty_ = true;
   reply_expected_ = expected;
}

inline ACE_UINT32 ACE_PIP_Data_Message::get_message_priority() const
{
   return message_priority_;
}

inline void ACE_PIP_Data_Message::set_message_priority(ACE_UINT32 priority)
{
   dirty_ = true;
   message_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Data_Message::get_destination_handler_ID() const
{
  return destination_handler_ID_;
}

inline void ACE_PIP_Data_Message::set_destination_handler_ID(ACE_UINT32 ID)
{
  destination_handler_ID_ = ID;
  dirty_ = true;
}

inline ACE_UINT32 ACE_PIP_Data_Message::get_source_handler_ID() const
{
  return source_handler_ID_;
}
   
inline void ACE_PIP_Data_Message::set_source_handler_ID(ACE_UINT32 ID)
{
  source_handler_ID_ = ID;
}

inline ACE_UINT32 ACE_PIP_Data_Message::get_source_site_ID() const
{
  return source_site_ID_;
}
   
inline void ACE_PIP_Data_Message::set_source_site_ID(ACE_UINT32 ID)
{
  source_site_ID_ = ID;
}

inline ACE_UINT32 ACE_PIP_Data_Message::get_destination_site_ID() const
{
  return destination_site_ID_;
}
   
inline void ACE_PIP_Data_Message::set_destination_site_ID(ACE_UINT32 ID)
{
  destination_site_ID_ = ID;
}

/**************************************************
 *
 *  ACE_PIP_Accel_Message - Inline Methods 
 *
 **************************************************/

inline ACE_UINT32 ACE_PIP_Accel_Message::get_old_priority() const
{
   return old_priority_;
}

inline void ACE_PIP_Accel_Message::set_old_priority(ACE_UINT32 priority)
{
   dirty_ = true;
   old_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Accel_Message::get_new_priority() const
{
   return new_priority_;
}

inline void ACE_PIP_Accel_Message::set_new_priority(ACE_UINT32 priority)
{
   dirty_ = true;
   new_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Accel_Message::get_destination_address() const
{
   return destination_address_;
}

inline void ACE_PIP_Accel_Message::set_destination_address(const ACE_UINT32& address)
{
   dirty_ = true;
   destination_address_ = address;
}

inline u_short ACE_PIP_Accel_Message::get_destination_port() const
{
  return destination_port_;
}

inline void ACE_PIP_Accel_Message::set_destination_port(u_short port)
{
  destination_port_ = port;
}

/**************************************************
 *
 *  ACE_PIP_Protocol_Message - Inline Methods 
 *
 **************************************************/

inline void ACE_PIP_Protocol_Message::
   set_message_type(ACE_PIP_Protocol_Message::Message_Type type)
{
   message_type_ = type;
   dirty_ = true;
}

inline ACE_PIP_Protocol_Message::Message_Type ACE_PIP_Protocol_Message::
   get_message_type() const
{
   return message_type_;
}

inline ACE_UINT64 ACE_PIP_Protocol_Message::get_message_id() const
{
   return message_id_;
}

inline void ACE_PIP_Protocol_Message::set_message_id(ACE_UINT64 id)
{
   dirty_ = true;
   message_id_ = id;
}

#endif

