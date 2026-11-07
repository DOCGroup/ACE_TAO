#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"
#include "Options.h"
#include "Supplier_Router.h"
#include "Message_Block_Owner.h"


#if defined (ACE_HAS_THREADS)

using SUPPLIER_FACTORY = Acceptor_Factory<Supplier_Handler, SUPPLIER_KEY>;

int
Supplier_Handler::open (void *a)
{
  SUPPLIER_FACTORY *af = static_cast<SUPPLIER_FACTORY *> (a);
  this->router_task_ = af->router ();
  return this->Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY>::open (a);
}

Supplier_Handler::Supplier_Handler (ACE_Thread_Manager *tm)
  : Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY> (tm)
{
}

// Create a new router and associate it with the REACTOR parameter..

Supplier_Router::Supplier_Router (ACE_Thread_Manager *tm)
  : SUPPLIER_ROUTER (tm)
{
}

// Handle incoming messages in a separate thread..

int
Supplier_Router::svc ()
{
  ACE_ASSERT (this->is_writer ());

  if (options.debug ())
    ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in %s\n", this->name ()));

  ACE_Message_Block *message_block {};
  while (this->getq (message_block) > 0)
    {
      UPIPE_Event_Server::Message_Block_Owner message {message_block};
      if (this->put_next (message_block) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) put_next failed in %s\n", this->name ()), -1);
      message.release ();
    }

  return 0;
}

// Initialize the Router..

int
Supplier_Router::open (void *)
{
  ACE_ASSERT (this->is_writer ());
  std::basic_string<ACE_TCHAR> file_arg {options.supplier_file ()};
  ACE_TCHAR *argv[] = {file_arg.data ()};

  if (this->init (1, argv) == -1)
    return -1;

  // Make this an active object.
  //   return this->activate (options.t_flags ());

  // Until that's done, return 1 to indicate that the object wasn't activated.
  return 1;
}

// Close down the router..

int
Supplier_Router::close (u_long)
{
  ACE_ASSERT (this->is_writer ());
  this->peer_map_.close ();
  this->msg_queue ()->deactivate();
  return 0;
}

// Send a MESSAGE_BLOCK to the supplier(s)..

int
Supplier_Router::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  ACE_ASSERT (this->is_writer ());

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }
  else
    {
//printf("supplier-Router is routing: send_peers\n");
      return this->send_peers (mb);
    }
}

// Return information about the Supplier_Router ACE_Module..

int
Supplier_Router::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR       buf[BUFSIZ];
  ACE_UPIPE_Addr  addr;
  ACE_TCHAR const * const module_name = this->name ();
  ACE_UPIPE_Acceptor &sa = this->acceptor_->acceptor ();

  if (sa.get_local_addr (addr) == -1)
    return -1;

  ACE_OS::sprintf (buf,
                   ACE_TEXT ("%") ACE_TEXT_PRIs
                   ACE_TEXT ("\t %") ACE_TEXT_PRIs
                   ACE_TEXT ("/ %") ACE_TEXT_PRIs,
                   module_name, ACE_TEXT ("upipe"),
                   ACE_TEXT ("# supplier router\n"));

  if (*strp == nullptr && (*strp = ACE_OS::strdup (module_name)) == nullptr)
    return -1;
  else
    ACE_OS::strncpy (*strp, module_name, length);

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (module_name));
}

#endif /* ACE_HAS_THREADS */
