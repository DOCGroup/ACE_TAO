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

#include "test_config.h"
#include "ace/RMCast/RMCast_UDP_Sender.h"
#include "ace/RMCast/RMCast_Fragment.h"

#include "ace/RMCast/RMCast_UDP_Receiver.h"
#include "ace/RMCast/RMCast_Sender_Proxy_Best_Effort.h"
#include "ace/RMCast/RMCast_Sender_Proxy_Factory.h"
#include "ace/RMCast/RMCast_Reassembly.h"

#include "ace/Task.h"

ACE_RCSID(tests, RMCast_UDP_Best_Effort_Test, "$Id$")

const size_t message_size = 8 * 1024;

// ****************************************************************

class Sender : public ACE_Task_Base
{
public:
  Sender (const ACE_INET_Addr &mcast_group);

  virtual int svc (void);

private:
  ACE_RMCast_UDP_Sender sender_;
  ACE_RMCast_Fragment fragment_;
};

// ****************************************************************

class Sender_Proxy_Factory : public ACE_RMCast_Sender_Proxy_Factory
{
public:
  Sender_Proxy_Factory (ACE_RMCast_Module *user_module);

  virtual ACE_RMCast_Sender_Proxy *create (void);
  virtual void destroy (ACE_RMCast_Sender_Proxy *);
  
private:
  ACE_RMCast_Module *user_module_;
};

// ****************************************************************

class Receiver : public ACE_RMCast_Module
{
public:
  Receiver (const ACE_INET_Addr &mcast_group);

  int handle_events (ACE_Time_Value *tv);
  // Invoke the UDP Receiver handle_events function

  virtual int open (void);
  virtual int put_data (ACE_RMCast::Data &data);

private:
  Sender_Proxy_Factory factory_;
  ACE_RMCast_UDP_Receiver udp_receiver_;
  ACE_INET_Addr mcast_group_;
};

// ****************************************************************

int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("RMCast_UDP_Best_Effort_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version(),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_INET_Addr mcast_group;
  mcast_group.set (12345, "224.9.9.1");

  Receiver receiver (mcast_group);
  if (receiver.open () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error in Receiver::open\n"), 1);

  Sender sender (mcast_group);
  if (sender.activate () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error in Sender::activate\n"), 1);

  ACE_Time_Value tv (120, 0);
  for (;;)
    {
      int r = receiver.handle_events (&tv);

      if (r < 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error in handle_events()\n"),
                            1);
        }
      if (tv == ACE_Time_Value::zero)
        break;
    }

  if (ACE_Thread_Manager::instance ()->wait () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error in Thread_Manager::wait\n"), 1);

  ACE_END_TEST;
  return 0;
}

// ****************************************************************

Sender_Proxy_Factory::Sender_Proxy_Factory (ACE_RMCast_Module *m)
  :  user_module_ (m)
{
}

ACE_RMCast_Sender_Proxy *
Sender_Proxy_Factory::create (void)
{
  ACE_RMCast_Module *top =
    new ACE_RMCast_Reassembly;
  top->next (this->user_module_);

  ACE_RMCast_Sender_Proxy *proxy =
    new ACE_RMCast_Sender_Proxy_Best_Effort (top);

  ACE_DEBUG ((LM_DEBUG, "Created proxy = %x\n", long(proxy)));
  return proxy;
}

void
Sender_Proxy_Factory::destroy (ACE_RMCast_Sender_Proxy *proxy)
{
  ACE_RMCast_Module *module = proxy->module ();
  delete module;
  delete proxy;
  ACE_DEBUG ((LM_DEBUG, "Destroyed proxy = %x\n", long(proxy)));
}

// ****************************************************************

Receiver::Receiver (const ACE_INET_Addr &mcast_group)
  :  factory_ (this)
  ,  udp_receiver_ (&factory_)
  ,  mcast_group_ (mcast_group)
{
}

int
Receiver::handle_events (ACE_Time_Value *tv)
{
  return this->udp_receiver_.handle_events (tv);
}

int
Receiver::open (void)
{
  if (this->udp_receiver_.subscribe (this->mcast_group_) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Error subscribing routine\n"), -1);
  return 0;
}

int
Receiver::put_data (ACE_RMCast::Data &data)
{
  if (data.total_size != message_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid message size (%d,%d,%d)\n",
                       data.sequence_number,
                       data.total_size,
                       data.fragment_offset), -1);
  if (data.fragment_offset != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Invalid message size (%d,%d,%d)\n",
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
                           "Unexpected byte at pos %d\n",
                           long(j - data.payload->rd_ptr ())), -1);
    }

  return 0;
}

// ****************************************************************

Sender::Sender (const ACE_INET_Addr &mcast_group)
  :  sender_ (mcast_group)
{
}

int
Sender::svc ()
{
  if (this->sender_.open () != 0)
    ACE_ERROR ((LM_ERROR, "Error in Sender::open()\n"));

  if (this->fragment_.next (&this->sender_) != 0)
    ACE_ERROR ((LM_ERROR, "Error in Fragment::next()\n"));

  ACE_Message_Block big_blob (message_size);
  big_blob.wr_ptr (message_size);

  char filler = 0;
  for (char* j = big_blob.rd_ptr (); j != big_blob.wr_ptr (); ++j)
    {
      *j = filler++;
    }

  for (int i = 0; i != 20; ++i)
    {
      ACE_RMCast::Data data;
      data.sequence_number = i;
      data.payload = &big_blob;
      this->fragment_.put_data (data);
    }
  return 0;
}
