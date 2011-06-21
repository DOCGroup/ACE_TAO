// $Id$
// -*- C++ -*-

#include "test_i.h"
#include "tao/PortableServer/ForwardRequestC.h"

test_i::test_i (CORBA::Short id,
                bool direct,
                CORBA::ORB_ptr orb)
  : id_ (id)
  , direct_ (direct)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , to_ (CORBA::Object::_nil ())
  , request_pass_count_ (0)
  , request_count_ (0)
{
}

void
test_i::forward (CORBA::Object_ptr to,
                 CORBA::ULong request_pass_count)
{
  this->to_ = CORBA::Object::_duplicate (to);
  this->request_pass_count_ = request_pass_count;
}

CORBA::Short
test_i::collocated_call (void)
{
  this->request_count_++;

  ACE_DEBUG ((LM_DEBUG,
              "Collocated call is executed in object with id %d.\n",
              this->id_));

  if (!CORBA::is_nil (this->to_.in ()))
    {
      if (this->request_count_ == this->request_pass_count_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "SERVER: Request %d will be forwarded "
                      "to object 'to'\nSERVER: via collocated_call().\n",
                      this->request_count_));

          // Throw forward exception
          throw PortableServer::ForwardRequest (this->to_.in ());
        }
    }

  return this->id_;
}
