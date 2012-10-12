// -*- C++ -*-
// $Id$

#include "TX_Object_i.h"

TX_Object_i::TX_Object_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    data_ ()
{
}

TX_Object_i::~TX_Object_i (void)
{
}

void
TX_Object_i::send (const DataSeq & data)
{
  this->data_ = data;

  ACE_DEBUG ((LM_DEBUG,
              "Received octet sequence of length:\t%u\n",
              data.length ()));
}

void
TX_Object_i::recv (DataSeq_out data)
{
  ACE_NEW_THROW_EX (data,
                    DataSeq,
                    CORBA::NO_MEMORY ());

  (*data) = this->data_;

  ACE_DEBUG ((LM_DEBUG,
              "Sending octet sequence of length:\t%u\n",
              data->length ()));
}

void
TX_Object_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
