// $Id$

// ============================================================================
//
// = DESCRIPTION
//    Unit test for the UDP sending module of the RMCast library.
//
// = AUTHORS
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#include "../test_config.h"
#include "ace/RMCast/RMCast_IO_UDP.h"
#include "ace/RMCast/RMCast_Fragment.h"

#include "ace/RMCast/RMCast_Module_Factory.h"
#include "ace/RMCast/RMCast_Fragment.h"
#include "ace/RMCast/RMCast_Reassembly.h"

#include "ace/Task.h"

ACE_RCSID(tests, RMCast_UDP_Best_Effort_Test, "$Id$")

const size_t message_size = 8 * 1024;
const int total_message_count = 40;

// ****************************************************************

class Sender_Factory : public ACE_RMCast_Module_Factory
{
public:
  Sender_Factory (void)
  {
  }

  virtual ACE_RMCast_Module *create (void)
  {
    return new ACE_RMCast_Reassembly;
  }

  virtual void destroy (ACE_RMCast_Module *module)
  {
    delete module;
  }
};

// ****************************************************************

class Receiver_Factory : public ACE_RMCast_Module_Factory
{
public:
  Receiver_Factory (ACE_RMCast_Module *module)
    : module_ (module)
  {
  }

  virtual ACE_RMCast_Module *create (void)
  {
    ACE_RMCast_Module *x = new ACE_RMCast_Reassembly;
    x->next (this->module_);
    return x;
  }

  virtual void destroy (ACE_RMCast_Module *module)
  {
    delete module;
  }

private:
  ACE_RMCast_Module *module_;
};

// ****************************************************************

class Sender : public ACE_Task_Base
{
public:
  Sender (const ACE_INET_Addr &mcast_group);

  virtual int svc (void);

private:
  Sender_Factory factory_;
  ACE_RMCast_IO_UDP io_udp_;
  ACE_RMCast_Fragment fragment_;

  ACE_INET_Addr mcast_group_;
};

// ****************************************************************

class Receiver : public ACE_RMCast_Module
{
public:
  Receiver (const ACE_INET_Addr &mcast_group);

  void dump (void);
  // Print the results of the test

  int handle_events (ACE_Time_Value *tv);
  // Invoke the UDP Receiver handle_events function

  virtual int open (void);
  virtual int data (ACE_RMCast::Data &data);

private:
  Receiver_Factory factory_;
  ACE_RMCast_IO_UDP io_udp_;

  ACE_INET_Addr mcast_group_;

  int message_count_;
};

// ****************************************************************

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_UDP_Best_Effort_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_INET_Addr mcast_group;
  mcast_group.set (12345, ACE_TEXT ("224.9.9.1"));

  Receiver receiver (mcast_group);
  if (receiver.open () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Error in Receiver::open\n")), 1);

  Sender sender (mcast_group);
  if (sender.activate () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Error in Sender::activate\n")), 1);

  ACE_Time_Value tv (120, 0);
  for (;;)
    {
      int r = receiver.handle_events (&tv);

      if (r < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Error in handle_events()\n")),
                            1);
        }
      if (tv == ACE_Time_Value::zero)
        break;
    }

  if (ACE_Thread_Manager::instance ()->wait () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error in Thread_Manager::wait\n")),
                       1);

  receiver.dump ();

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

Receiver::Receiver (const ACE_INET_Addr &mcast_group)
  :  factory_ (this)
  ,  io_udp_ (&factory_)
  ,  mcast_group_ (mcast_group)
  ,  message_count_ (0)
{
}

int
Receiver::handle_events (ACE_Time_Value *tv)
{
  return this->io_udp_.handle_events (tv);
}

int
Receiver::open (void)
{
  if (this->io_udp_.subscribe (this->mcast_group_) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error in IO_UDP::subscribe\n")),
                      -1);
  return 0;
}

int
Receiver::data (ACE_RMCast::Data &data)
{
  if (data.total_size != message_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Invalid message size (%d,%d,%d)\n"),
                       data.sequence_number,
                       data.total_size,
                       data.fragment_offset), -1);
  if (data.fragment_offset != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Invalid message size (%d,%d,%d)\n"),
                       data.sequence_number,
                       data.total_size,
                       data.fragment_offset), -1);

  char expected = 0;
  for (char *j = data.payload->rd_ptr ();
       j != data.payload->wr_ptr ();
       ++j)
    {
      if (*j != expected++)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Unexpected byte at pos %d\n"),
                           long(j - data.payload->rd_ptr ())), -1);
    }

  this->message_count_++;

  return 0;
}

void
Receiver::dump (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message count = %d/%d\n"),
              this->message_count_,
              total_message_count));
}

// ****************************************************************

Sender::Sender (const ACE_INET_Addr &mcast_group)
  :  io_udp_ (&factory_)
  ,  mcast_group_ (mcast_group)
{
}

int
Sender::svc ()
{
  if (this->fragment_.next (&this->io_udp_) != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error in Fragment::next()\n")));

  if (this->io_udp_.subscribe (this->mcast_group_) != 0)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error in IO_UDP::subscribe()\n")));

  ACE_Message_Block big_blob (message_size);
  big_blob.wr_ptr (message_size);

  char filler = 0;
  for (char* j = big_blob.rd_ptr (); j != big_blob.wr_ptr (); ++j)
    {
      *j = filler++;
    }

  for (int i = 0; i != total_message_count; ++i)
    {
      ACE_RMCast::Data data;
      data.sequence_number = i;
      data.payload = &big_blob;
      this->fragment_.data (data);
    }
  return 0;
}
