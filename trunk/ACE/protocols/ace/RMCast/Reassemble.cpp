// file      : ace/RMCast/Reassemble.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#include "Reassemble.h"
#include "ace/OS_NS_stdlib.h"

/*
#include <iostream>
using std::cerr;
using std::endl;
*/

namespace ACE_RMCast
{
  Reassemble::
  Reassemble (Parameters const& params)
      : params_ (params)
  {
  }

  void Reassemble::
  recv (Message_ptr m)
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
            new_msg->add (Profile_ptr (new_data.release ()));

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
