// -*- C++ -*-

#include "TX_Object_i.h"


ACE_RCSID (Big_Request,
           TX_Object_i,
           "$Id$")


TX_Object_i::TX_Object_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    data_ ()
{
}

TX_Object_i::~TX_Object_i (void)
{
}

void
TX_Object_i::send (const DataSeq & data
                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->data_ = data;

  ACE_DEBUG ((LM_DEBUG,
              "Received octet sequence of length:\t%u\n",
              data.length ()));
}

void
TX_Object_i::recv (DataSeq_out data
                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (data,
                    DataSeq,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  (*data) = this->data_;

  ACE_DEBUG ((LM_DEBUG,
              "Sending  octet sequence of length:\t%u\n",
              data->length ()));
}

void
TX_Object_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
