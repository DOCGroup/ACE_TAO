// author    : Boris Kolpackov <boris@kolpackov.net>
// $Id$

#include "Fragment.h"

/*
#include <iostream>
using std::cerr;
using std::endl;
*/

namespace ACE_RMCast
{
  Fragment::
  Fragment (Parameters const& params)
      : params_ (params),
        sn_ (1)
  {
  }

  void Fragment::send (Message_ptr m)
  {
    if (Data const* data = static_cast<Data const*> (m->find (Data::id)))
    {
      size_t max_payload_size (
        params_.max_packet_size () - max_service_size);

      if (data->size () <= max_payload_size)
      {
        u64 sn;
        {
          Lock l (mutex_);
          sn = sn_++;
        }

        m->add (Profile_ptr (new SN (sn)));

        out_->send (m);
        return;
      }

      char const* p = data->buf ();
      size_t size (data->size ());

      // Need fragmentation.
      //
      u32 packets (size / max_payload_size + (size % max_payload_size ? 1 : 0));

      // cerr << "size : " << size << endl
      //      << "packs: " << packets << endl;

      for (u32 i (1); i <= packets; ++i)
      {
        Message_ptr part (new Message);

        size_t s (i == packets ? size % max_payload_size : max_payload_size);

        // cerr << "pack: " << s << endl;

        u64 sn;
        {
          Lock l (mutex_);
          sn = sn_++;
        }

        part->add (Profile_ptr (new SN (sn)));
        part->add (Profile_ptr (new Part (i, packets, size)));
        part->add (Profile_ptr (new Data (p, s)));

        out_->send (part);

        p += s;
      }
    }
  }
}
