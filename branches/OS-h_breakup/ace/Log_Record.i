/* -*- C++ -*- */
// $Id$

// Log_Record.i

#include "ace/Trace.h"
#include "ace/Global_Macros.h"
#include "ace/Time_Value.h"
#include "ace/OS_String.h"

ASYS_INLINE
ACE_Log_Record::~ACE_Log_Record (void)
{
}

ASYS_INLINE void
ACE_Log_Record::encode (void)
{
  ACE_TRACE ("ACE_Log_Record::encode");
  this->length_ = ACE_HTONL (this->length_);
  this->type_ = ACE_HTONL (this->type_);
  // Make sure we don't enclose the sec() and usec() fields until
  // they've been normalized.
  this->secs_ = ACE_HTONL (this->secs_);
  this->usecs_ = ACE_HTONL (this->usecs_);
  this->pid_ = ACE_HTONL (this->pid_);
}

ASYS_INLINE void
ACE_Log_Record::decode (void)
{
  ACE_TRACE ("ACE_Log_Record::decode");
  ACE_Time_Value tv (ACE_NTOHL (this->secs_),
                     ACE_NTOHL (this->usecs_));

  this->secs_ = tv.sec ();
  this->usecs_ = tv.usec ();
  this->type_ = ACE_NTOHL (this->type_);
  this->pid_ = ACE_NTOHL (this->pid_);
  this->length_ = ACE_NTOHL (this->length_);
}

ASYS_INLINE long
ACE_Log_Record::type (void) const
{
  ACE_TRACE ("ACE_Log_Record::type");
  return (long) this->type_;
}

ASYS_INLINE void
ACE_Log_Record::type (long t)
{
  ACE_TRACE ("ACE_Log_Record::type");
  this->type_ = (ACE_UINT32) t;
}

ASYS_INLINE long
ACE_Log_Record::length (void) const
{
  ACE_TRACE ("ACE_Log_Record::length");
  return (long) this->length_;
}

ASYS_INLINE void
ACE_Log_Record::length (long l)
{
  ACE_TRACE ("ACE_Log_Record::length");
  this->length_ = ACE_static_cast (ACE_UINT32, l);
}

ASYS_INLINE ACE_Time_Value 
ACE_Log_Record::time_stamp (void) const
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  return ACE_Time_Value ((long) this->secs_, (long) this->usecs_);
}

ASYS_INLINE void
ACE_Log_Record::time_stamp (const ACE_Time_Value &ts)
{
  ACE_TRACE ("ACE_Log_Record::time_stamp");
  this->secs_ = (ACE_UINT32) ts.sec ();
  this->usecs_ = (ACE_UINT32) ts.usec ();
}

ASYS_INLINE long
ACE_Log_Record::pid (void) const
{
  ACE_TRACE ("ACE_Log_Record::pid");
  return (long) this->pid_;
}

ASYS_INLINE void
ACE_Log_Record::pid (long p)
{
  ACE_TRACE ("ACE_Log_Record::pid");
  this->pid_ = (ACE_UINT32) p;
}

ASYS_INLINE const ACE_TCHAR *
ACE_Log_Record::msg_data (void) const
{
  ACE_TRACE ("ACE_Log_Record::msg_data");
  return this->msg_data_;
}

ASYS_INLINE size_t
ACE_Log_Record::msg_data_len (void) const
{
  ACE_TRACE ("ACE_Log_Record::msg_data_len");
  return ACE_OS_String::strlen (this->msg_data_) + 1;
}
