//
// $Id$
//

#include "Hello_Impl.h"

ACE_Atomic_Op< TAO_SYNCH_MUTEX, u_long > Number_of_Problems = 0uL;

UIPMC_Object_Impl::UIPMC_Object_Impl (CORBA::ULong payload,
                                      CORBA::ULong clients,
                                      CORBA::ULong calls)
  : payload_ (payload)
  , clients_ (clients)
  , calls_ (calls)
{
}

UIPMC_Object_Impl::~UIPMC_Object_Impl (void)
{
  if (this->received_.current_size () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: expected %d clients but only %d encountered\n"),
                  this->clients_, this->received_.current_size ()));
      ++Number_of_Problems;
      return;
    }

  for (CORBA::ULong i = 0; i < this->clients_; ++i)
    {
      CORBA::ULong count = 0;
      this->received_.find (Test::ClientIDs[i], count);

      if (count != this->calls_)
        // This perfectly ok for MIOP to lose messages.
        // So, this is not an error.
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("DEBUG: expected %d messages from '%c' client ")
                    ACE_TEXT ("but only %d encountered\n"),
                    this->calls_, Test::ClientIDs[i], count));
      ++Number_of_Problems;
    }
}

void
UIPMC_Object_Impl::process (Test::Octets const &payload)
{
  if (this->payload_ != payload.length ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: expected %d but received %d ")
                  ACE_TEXT ("sequence length\n"),
                  this->payload_, payload.length ()));
      ++Number_of_Problems;
      return;
    }

  CORBA::ULong count = 0;
  CORBA::Octet c = payload[0];
  this->received_.find (c, count);

  Test::Octets seq (this->payload_);
  seq.length (this->payload_);
  CORBA::Octet *buff = seq.get_buffer ();
  ACE_OS::memset (buff, c, this->payload_);
  char *one = static_cast<char *> (static_cast<void *> (buff));
  char const *two = static_cast<char const *> (
    static_cast<void const *> (payload.get_buffer ()));
  if (ACE_OS::strncmp (one, two, this->payload_) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: received malformed message from client '%c'\n"),
                  c));
      ++Number_of_Problems;
      return;
    }

  if (ACE_OS::strchr (Test::ClientIDs, c) == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: client id '%c' doesn't match any known value\n"),
                  c));
      ++Number_of_Problems;
      return;
    }

  ++count;
  if (this->received_.rebind (c, count) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: cann't rebind received count\n")));
      ++Number_of_Problems;
    }
}

Hello_Impl::Hello_Impl (CORBA::ORB_ptr orb,
                        Test::UIPMC_Object_ptr obj)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , obj_ (Test::UIPMC_Object::_duplicate (obj))
{
}

Test::UIPMC_Object_ptr
Hello_Impl::get_object (void)
{
  return Test::UIPMC_Object::_duplicate (this->obj_.in ());
}

void
Hello_Impl::shutdown (void)
{
  try
    {
      this->orb_->shutdown (0);
    }
  catch (const CORBA::Exception& ex)
    {
      ++Number_of_Problems;
      ex._tao_print_exception ("Exception caught in shutdown():");
    }
}
