// $Id$

#include "ace/os_include/os_assert.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"
#include "Supplier_Router.h"
#include "Options.h"



// Handle outgoing messages in a separate thread.

int
Supplier_Router::svc (void)
{
  assert (this->is_writer ());

  ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in Supplier_Router\n"));

  for (ACE_Message_Block *mb = 0;
       this->getq (mb) >= 0;
       )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) warning: Supplier_Router is "
                  "forwarding a message via send_peers\n"));

      // Broadcast the message to the Suppliers, even though this is
      // "incorrect" (assuming a oneway flow of events from Suppliers
      // to Consumers)!

      if (this->context ()->send_peers (mb) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) send_peers failed in Supplier_Router\n"),
                      -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) stopping svc in Supplier_Router\n"));
  return 0;
}

Supplier_Router::Supplier_Router (Peer_Router_Context *prc)
  : Peer_Router (prc)
{
  // Increment the reference count.
  this->context ()->duplicate ();
}

// Initialize the Supplier Router.

int
Supplier_Router::open (void *)
{
  if (this->is_reader ())
    {
      // Set the <Peer_Router_Context> to point back to us so that all
      // the Peer_Handler's <put> their incoming <Message_Blocks> to
      // our reader Task.
      this->context ()->peer_router (this);
      return 0;
    }

  else // if (this->is_writer ()
    {
      // Increment the reference count.
      this->context ()->duplicate ();

      // Make this an active object to handle the error cases in a
      // separate thread.
      return this->activate (Options::instance ()->t_flags ());
    }
}

// Close down the router.

int
Supplier_Router::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) closing Supplier_Router %s\n",
              this->is_reader () ? "reader" : "writer"));

  if (this->is_writer ())
    // Inform the thread to shut down.
    this->msg_queue ()->deactivate ();

  // Both writer and reader call release(), so the context knows when
  // to clean itself up.
  this->context ()->release ();
  return 0;
}

// Send an <ACE_Message_Block> to the supplier(s).

int
Supplier_Router::put (ACE_Message_Block *mb,
                      ACE_Time_Value *)
{
  // Perform the necessary control operations before passing
  // the message up the stream.

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }

  // If we're the reader then we are responsible for pass messages up
  // to the next Module's reader Task.  Note that in a "real"
  // application this is likely where we'd take a look a the actual
  // information that was in the message, e.g., in order to figure out
  // what operation it was and what it's "parameters" where, etc.
  else if (this->is_reader ())
    return this->put_next (mb);

  else // if (this->is_writer ())
    {
      // Someone is trying to write to the Supplier.  In this
      // implementation this is considered an "error."  However, we'll
      // just go ahead and forward the message to the Supplier (who
      // hopefully is prepared to receive it).
      ACE_DEBUG ((LM_WARNING,
                  "(%t) warning: sending to a Supplier\n"));

      // Queue up the message to processed by <Supplier_Router::svc>.
      // Since we don't expect to be getting many of these messages,
      // we queue them up and run them in a separate thread to avoid
      // taxing the main thread.
      return this->putq (mb);
    }
}

// Return information about the <Supplier_Router>.
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
#  define FMTSTR  ACE_TEXT ("%s\t %d/%s %s (%s)\n")
#else
#  define FMTSTR  ACE_TEXT ("%ls\t %d/%ls %ls (%ls)\n")
#endif /* ACE_WIN32 || !ACE_USES_WCHAR */

int
Supplier_Router::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr addr;
  const ACE_TCHAR *module_name = this->name ();

  if (this->context ()->acceptor ().get_local_addr (addr) == -1)
    return -1;

  ACE_OS::sprintf (buf,
                   FMTSTR,
                   module_name,
                   addr.get_port_number (),
                   ACE_TEXT ("tcp"),
                   ACE_TEXT ("# supplier router"),
                   this->is_reader () ?
                     ACE_TEXT ("reader") : ACE_TEXT ("writer"));
  if (*strp == 0 && (*strp = ACE_OS::strdup (module_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, module_name, length);

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (module_name));
}
