// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "Reassemble.h"
#include "ace/OS_NS_stdlib.h"

namespace ACE_RMCast
{
  Reassemble::
  Reassemble (Parameters const& params)
      : params_ (params)
  {
  }

  void Reassemble::recv (Message_ptr m)
  {
    Map::ENTRY* e;
    Address from (
      static_cast<From const*> (m->find (From::id))->address ());

    if (Data const* data = static_cast<Data const*> (m->find (Data::id)))
    {
      if (Part const* part = static_cast<Part const*> (m->find (Part::id)))
      {
        if (map_.find (from, e) == -1)
        {
          // First part of the message.
          //

          if (part->num () != 1)
          {
            // We assume that we received NoData for one of the preceding
            // fragments. Ignore this one.
            return;
          }

          Data_ptr new_data (new Data (data->buf (),
                                       static_cast<size_t> (data->size ()),
                                       static_cast<size_t> (part->total_size ())));

          //std::cerr << "part->total_size (): " << part->total_size () << endl;

          map_.bind (from, new_data);
        }
        else
        {
          // Next part of the message.
          //

          if (part->num () == 1)
            ACE_OS::abort ();


          Data const* data = static_cast<Data const*> (m->find (Data::id));

          Data_ptr& new_data = e->int_id_;

          ACE_OS::memcpy (new_data->buf () + new_data->size (),
                          data->buf (),
                          data->size ());

          //std::cerr << "data->size (): " << data->size () << endl
          //          << "new_data->size (): " << new_data->size () << endl
          //          << "new_data->capa (): " << new_data->capacity () << endl;

          new_data->size (new_data->size () + data->size ());


          if (part->num () == part->of ())
          {
            // Reassembly is complete.
            //
            if (part->total_size () != new_data->size ())
              ACE_OS::abort ();

            Message_ptr new_msg (new Message ());

            Address to (
              static_cast<To const*> (m->find (To::id))->address ());

            new_msg->add (Profile_ptr (new To (to)));
            new_msg->add (Profile_ptr (new From (from)));
            /*
             * Heads up... we need to add the new_data to new_msg then
             * unbind the entry that maps to new_data, which will decrement
             * its reference count. If the bound/refcounted pointer acted
             * polymorphically like a regular pointer does, we'd be able to
             * just pass new_data to add(Profile_Ptr) and it would work.
             * However, Profile_Ptr and Data_Ptr are not compatible, but
             * we can use the secret knowledge that both are instances of the
             * same template and that the pointers they contain really are
             * hierarchically compatible, and do this funky cast to get
             * the result we want.
             */
            //new_msg->add (*(reinterpret_cast<Profile_ptr*> (&new_data)));

            new_msg->add (Profile_ptr (new_data));

            map_.unbind (from);

            in_->recv (new_msg);
          }
        }
      }
      else
      {
        // Non-fragmented message. Make sure we are in the consistent state
        // and forward it up.
        //
        if (map_.find (from, e) != -1)
          ACE_OS::abort ();

        in_->recv (m);
      }
    }
    else if (m->find (NoData::id) != 0)
    {
      if (map_.find (from, e) != -1)
      {
        // We already received some fragments. Clean everyhting up.
        //
        map_.unbind (from);
      }

      in_->recv (m);
    }
  }
}
