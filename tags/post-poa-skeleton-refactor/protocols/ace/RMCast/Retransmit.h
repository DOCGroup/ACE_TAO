// file      : ace/RMCast/Retransmit.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_RETRANSMIT_H
#define ACE_RMCAST_RETRANSMIT_H

#include <ace/Hash_Map_Manager.h>

#include <ace/Thread_Manager.h>

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

namespace ACE_RMCast
{
  class Retransmit : public Element
  {
  public:
    Retransmit ();

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
          : data_ (), count_ (0)
      {
      }

      Descr (Data_ptr d)
          : data_ (d), count_ (0)
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

      // It would be logical to return data_ptr but ACE ref_auto_ptr
      // hasn't learned how to convert between pointers yet.
      //
      Profile_ptr
      data () const
      {
        return Profile_ptr (new Data (*data_));
      }

    private:
      Data_ptr data_;
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
    Queue queue_;
    Mutex mutex_;

    ACE_Thread_Manager tracker_mgr_;
  };
}


#endif  // ACE_RMCAST_RETRANSMIT_H
