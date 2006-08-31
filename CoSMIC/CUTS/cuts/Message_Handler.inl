// $Id$

//
// error_message_count
//
CUTS_INLINE
size_t CUTS_Message_Handler::error_message_count (void) const
{
  return this->errors_;
}

//
// warning_message_count
//
CUTS_INLINE
size_t CUTS_Message_Handler::warning_message_count (void) const
{
  return this->warnings_;
}

//
// error_message
//
CUTS_INLINE
void CUTS_Message_Handler::error_message (const char *)
{
  this->errors_ ++;
}

//
// warning_message
//
CUTS_INLINE
void CUTS_Message_Handler::warning_message (const char *)
{
  this->warnings_ ++;
}
