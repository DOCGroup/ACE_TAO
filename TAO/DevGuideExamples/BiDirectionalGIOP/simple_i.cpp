#include "bidir_giop_pch.h"

#include "simple_i.h"

#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

#include <iostream>

Simple_i::Simple_i (CORBA::ORB_ptr orb, int callback_count)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    callback_count_ (callback_count),
    callback_ {}
{
}

CORBA::Long
Simple_i::test_method (CORBA::Boolean do_callback)
{
  if (do_callback)
    this->ready_for_callback_ = true;
  return 0;
}

void
Simple_i::callback_object (Callback_ptr cb)
{
  this->callback_ = Callback::_duplicate (cb);
}

void
Simple_i::shutdown ()
{
  CORBA::Boolean const wait = false;
  this->orb_->shutdown (wait);
}

bool
Simple_i::call_client ()
{
  if (!this->ready_for_callback_)
    return false;

  this->ready_for_callback_ = false;
  for (int iteration = 0; iteration < this->callback_count_; ++iteration)
    {
      this->callback_->callback_method ();
      if (this->orb_->orb_core ()->lane_resources ().transport_cache ()
          .current_size () > 1)
        {
          std::cerr << "The connection cache has grown. "
                    << "Bidirectional GIOP did not work; aborting." << std::endl;
          ACE_OS::abort ();
        }
    }

  this->callback_->shutdown ();
  return true;
}

