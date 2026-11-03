/* -*- c++ -*- */
#ifndef JAWS_SYNCH_IO_H
#define JAWS_SYNCH_IO_H

#include "ace/Singleton.h"

#include "jaws3/Jaws_IO.h"

class JAWS_Export JAWS_Synch_IO : public JAWS_IO_Impl
{
public:
  static JAWS_Synch_IO * instance ()
  {
    return ACE_Singleton<JAWS_Synch_IO, ACE_SYNCH_MUTEX>::instance ();
  }

  void send ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , void *act = nullptr
            ) override;

  void recv ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , void *act = nullptr
            ) override;

  void transmit ( ACE_HANDLE handle
                , ACE_HANDLE source
                , JAWS_Event_Completer *completer
                , void *act = nullptr
                , ACE_Message_Block *header = nullptr
                , ACE_Message_Block *trailer = nullptr
                ) override;

  void send ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , const ACE_Time_Value &tv
            , void *act = nullptr
            ) override;

  void recv ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , const ACE_Time_Value &tv
            , void *act = nullptr
            ) override;

  void transmit ( ACE_HANDLE handle
                , ACE_HANDLE source
                , JAWS_Event_Completer *completer
                , const ACE_Time_Value &tv
                , void *act = nullptr
                , ACE_Message_Block *header = nullptr
                , ACE_Message_Block *trailer = nullptr
                ) override;
};

#endif /* JAWS_SYNCH_IO_H */
