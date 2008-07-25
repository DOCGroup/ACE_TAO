/**************************************************
 *
 *  ACE_PIP_Message - Inline Methods 
 *
 **************************************************/
inline ACE_PIP_Message* ACE_PIP_Message::next()
{
   return this->next_;
}

inline void ACE_PIP_Message::next(ACE_PIP_Message* message)
{
   this->next_ = message;
}

inline ACE_PIP_Message* ACE_PIP_Message::release_next()
{
   ACE_PIP_Message* temp = this->next_;
   this->next_ = 0;
   return temp;
}

inline ACE_Message_Block* ACE_PIP_Message::block() 
{
   return this->block_;
}

inline ACE_Message_Block* ACE_PIP_Message::release_block()
{
   ACE_Message_Block* temp_block = this->block_;
   this->block_ = 0;
   this->dirty_ = true;
   return temp_block;
}

/**************************************************
 *
 *  ACE_PIP_Data_Message - Inline Methods 
 *
 **************************************************/

inline bool ACE_PIP_Data_Message::reply_expected() const
{
   return this->reply_expected_;
}

inline void ACE_PIP_Data_Message::reply_expected(bool expected)
{
   this->dirty_ = true;
   this->reply_expected_ = expected;
}

inline ACE_UINT32 ACE_PIP_Data_Message::message_priority() const
{
   return this->message_priority_;
}

inline void ACE_PIP_Data_Message::message_priority(ACE_UINT32 priority)
{
   this->dirty_ = true;
   this->message_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Data_Message::destination_handler_ID() const
{
  return this->destination_handler_id_;
}

inline void ACE_PIP_Data_Message::destination_handler_ID(ACE_UINT32 ID)
{
  this->destination_handler_id_ = ID;
  this->dirty_ = true;
}

inline ACE_UINT32 ACE_PIP_Data_Message::source_handler_ID() const
{
  return this->source_handler_id_;
}
   
inline void ACE_PIP_Data_Message::source_handler_ID(ACE_UINT32 ID)
{
  this->source_handler_id_ = ID;
}

inline ACE_UINT32 ACE_PIP_Data_Message::source_site_ID() const
{
  return this->source_site_id_;
}
   
inline void ACE_PIP_Data_Message::source_site_ID(ACE_UINT32 ID)
{
  this->source_site_id_ = ID;
}

inline ACE_UINT32 ACE_PIP_Data_Message::destination_site_ID() const
{
  return this->destination_site_id_;
}
   
inline void ACE_PIP_Data_Message::destination_site_ID(ACE_UINT32 ID)
{
  this->destination_site_id_ = ID;
}

/**************************************************
 *
 *  ACE_PIP_Accel_Message - Inline Methods 
 *
 **************************************************/

inline ACE_UINT32 ACE_PIP_Accel_Message::old_priority() const
{
   return old_priority_;
}

inline void ACE_PIP_Accel_Message::old_priority(ACE_UINT32 priority)
{
   this->dirty_ = true;
   old_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Accel_Message::new_priority() const
{
   return new_priority_;
}

inline void ACE_PIP_Accel_Message::new_priority(ACE_UINT32 priority)
{
   this->dirty_ = true;
   new_priority_ = priority;
}

inline ACE_UINT32 ACE_PIP_Accel_Message::destination_address() const
{
   return destination_address_;
}

inline void ACE_PIP_Accel_Message::destination_address(const ACE_UINT32& address)
{
   this->dirty_ = true;
   destination_address_ = address;
}

inline u_short ACE_PIP_Accel_Message::destination_port() const
{
  return destination_port_;
}

inline void ACE_PIP_Accel_Message::destination_port(u_short port)
{
  destination_port_ = port;
}

/**************************************************
 *
 *  ACE_PIP_Protocol_Message - Inline Methods 
 *
 **************************************************/

inline void ACE_PIP_Protocol_Message::
   message_type(ACE_PIP_Protocol_Message::Message_Type type)
{
   message_type_ = type;
   this->dirty_ = true;
}

inline ACE_PIP_Protocol_Message::Message_Type ACE_PIP_Protocol_Message::message_type() const
{
   return message_type_;
}

inline ACE_UINT64 ACE_PIP_Protocol_Message::message_id() const
{
   return message_id_;
}

inline void ACE_PIP_Protocol_Message::message_id(ACE_UINT64 id)
{
   this->dirty_ = true;
   message_id_ = id;
}

inline ACE_PIP_Message* ACE_PIP_Protocol_Message::next()
{
  return this->next_;
}
