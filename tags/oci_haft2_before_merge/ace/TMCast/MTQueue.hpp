// file      : TMCast/MTQueue.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef TMCAST_MT_QUEUE_HPP
#define TMCAST_MT_QUEUE_HPP

#include "ace/Auto_Ptr.h"
#include "ace/Unbounded_Set.h"
#include "ace/Unbounded_Queue.h"

namespace TMCast
{
  template <typename T,
            typename M,
            typename C,
            typename Q = ACE_Unbounded_Queue<T> >
  class MTQueue
  {
  public:
    typedef T ElementType;
    typedef M MutexType;
    typedef C ConditionalType;
    typedef Q QueueType;

  public:

    MTQueue (std::size_t hint = 0)
        : mutexp_ (new MutexType),
          mutex_ (*mutexp_),
          // queue_ (hint),
          queue_ (),
          signal_ (false)
    {
    }

    MTQueue (MutexType& mutex, std::size_t hint = 0)
        : mutexp_ (),
          mutex_ (mutex),
          // queue_ (hint),
          queue_ (),
          signal_ (false)
    {
    }

  public:
    bool
    empty () const
    {
      return queue_.is_empty ();
    }

    std::size_t
    size () const
    {
      return queue_.size ();
    }

    // typedef typename QueueType::Empty Empty;

    class Empty {};

    T&
    front ()
    {
      ACE_Unbounded_Queue_Iterator<T> f (queue_);
      T* tmp;
      if (!f.next (tmp)) throw Empty ();

      return *tmp;
    }


    T const&
    front () const
    {
      ACE_Unbounded_Queue_Const_Iterator<T> f (queue_);
      T* tmp;
      if (!f.next (tmp)) throw Empty ();

      return *tmp;
    }

    /*
      T&
      back ()
      {
      return queue_.back ();
      }


      T const&
      back () const
      {
      return queue_.back ();
      }
    */

    void
    push (T const& t)
    {
      signal_ = empty ();
      queue_.enqueue_tail (t);
    }

    void
    pop ()
    {
      T junk;
      queue_.dequeue_head (junk);
    }

  public:
    void
    lock () const
    {
      mutex_.acquire ();
    }

    void
    unlock () const
    {
      if (signal_)
      {
        signal_ = false;

        for (ConditionalSetConstIterator_ i (cond_set_);
             !i.done ();
             i.advance ())
        {
          ConditionalType** c;

          i.next (c);

          (*c)->signal ();
        }
      }

      mutex_.release ();
    }

    void
    subscribe (ConditionalType& c)
    {
      //@@ should check for duplicates
      //
      cond_set_.insert (&c);
    }

    void
    unsubscribe (ConditionalType& c)
    {
      //@@ should check for absence
      //
      cond_set_.remove (&c);
    }

  private:
    auto_ptr<MutexType> mutexp_;
    mutable MutexType& mutex_;
    QueueType queue_;

    typedef
    ACE_Unbounded_Set<ConditionalType*>
    ConditionalSet_;

    typedef
    ACE_Unbounded_Set_Const_Iterator<ConditionalType*>
    ConditionalSetConstIterator_;

    ConditionalSet_ cond_set_;

    mutable bool signal_;
  };
}

#endif // TMCAST_MT_QUEUE_HPP
