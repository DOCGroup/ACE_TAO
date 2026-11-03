/* -*- c++ -*- */
#ifndef JAWS_IO_H
#define JAWS_IO_H

#include "ace/Message_Block.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"

#include "jaws3/Export.h"


class JAWS_IO;
class JAWS_Event_Completer;


class JAWS_Export JAWS_IO_Impl
// = TITLE
//     IO inteface for the implementation classes on the RHS of the
//     Bridge Pattern.
{
public:
  virtual ~JAWS_IO_Impl () = default;

  // = Regular IO methods.

  virtual void send ( ACE_HANDLE handle
                    , ACE_Message_Block *mb
                    , JAWS_Event_Completer *completer
                    , void *act = nullptr
                    ) = 0;

  virtual void recv ( ACE_HANDLE handle
                    , ACE_Message_Block *mb
                    , JAWS_Event_Completer *completer
                    , void *act = nullptr
                    ) = 0;

  virtual void transmit ( ACE_HANDLE handle
                        , ACE_HANDLE source
                        , JAWS_Event_Completer *completer
                        , void *act = nullptr
                        , ACE_Message_Block *header = nullptr
                        , ACE_Message_Block *trailer = nullptr
                        ) = 0;

  // = Timed IO methods.

  virtual void send ( ACE_HANDLE handle
                    , ACE_Message_Block *mb
                    , JAWS_Event_Completer *completer
                    , const ACE_Time_Value &tv
                    , void *act = nullptr
                    ) = 0;

  virtual void recv ( ACE_HANDLE handle
                    , ACE_Message_Block *mb
                    , JAWS_Event_Completer *completer
                    , const ACE_Time_Value &tv
                    , void *act = nullptr
                    ) = 0;

  virtual void transmit ( ACE_HANDLE handle
                        , ACE_HANDLE source
                        , JAWS_Event_Completer *completer
                        , const ACE_Time_Value &tv
                        , void *act = nullptr
                        , ACE_Message_Block *header = nullptr
                        , ACE_Message_Block *trailer = nullptr
                        ) = 0;
};


class JAWS_Export JAWS_IO
{
public:
  explicit JAWS_IO (JAWS_IO_Impl *impl = nullptr);

  ~JAWS_IO ();

  static JAWS_IO * instance ()
  {
    return ACE_Singleton<JAWS_IO, ACE_SYNCH_MUTEX>::instance ();
  }

  void send ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , void *act = nullptr
            );

  void recv ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , void *act = nullptr
            );

  void transmit ( ACE_HANDLE handle
                , ACE_HANDLE source
                , JAWS_Event_Completer *completer
                , void *act = nullptr
                , ACE_Message_Block *header = nullptr
                , ACE_Message_Block *trailer = nullptr
                );

  void send ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , const ACE_Time_Value &tv
            , void *act = nullptr
            );

  void recv ( ACE_HANDLE handle
            , ACE_Message_Block *mb
            , JAWS_Event_Completer *completer
            , const ACE_Time_Value &tv
            , void *act = nullptr
            );

  void transmit ( ACE_HANDLE handle
                , ACE_HANDLE source
                , JAWS_Event_Completer *completer
                , const ACE_Time_Value &tv
                , void *act = nullptr
                , ACE_Message_Block *header = nullptr
                , ACE_Message_Block *trailer = nullptr
                );


private:
  JAWS_IO_Impl *impl_;
};

#endif /* JAWS_IO_H */
