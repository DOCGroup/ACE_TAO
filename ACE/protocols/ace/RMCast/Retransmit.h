// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#ifndef ACE_RMCAST_RETRANSMIT_H
#define ACE_RMCAST_RETRANSMIT_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Thread_Manager.h"

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"
#include "Parameters.h"

namespace ACE_RMCast
{
  class Retransmit : public Element
  {
  public:
    Retransmit (Parameters const& params);

    virtual void
    out_start (Out_Element* out);

    virtual void
    out_stop ();

  public:
    virtual void
    send (Message_ptr m);

    virtual void
    recv (Message_ptr m);

  private:
    struct Descr
    {
      // Shouldn't be available but ACE_Hash_Map needs it.
      //
      Descr ()
          : msg_ (), count_ (0)
      {
      }

      Descr (Message_ptr msg)
          : msg_ (msg), count_ (0)
      {
      }

      unsigned long
      inc ()
      {
        return ++count_;
      }

      void
      reset ()
      {
        count_ = 0;
      }

      Message_ptr
      message () const
      {
        return msg_->clone ();
      }

    private:
      Message_ptr msg_;
      unsigned long count_;
    };

    typedef
    ACE_Hash_Map_Manager<u64, Descr, ACE_Null_Mutex>
    Queue;

  private:
    void
    track ();

    static ACE_THR_FUNC_RETURN
    track_thunk (void* obj);

  private:
    Parameters const& params_;

    Queue queue_;
    Mutex mutex_;
    Condition cond_;

    bool stop_;
    ACE_Thread_Manager tracker_mgr_;
  };
}


#endif  // ACE_RMCAST_RETRANSMIT_H
