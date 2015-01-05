// $Id$

#include "ace/os_include/os_assert.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Truncate.h"
#include "Consumer_Router.h"
#include "Options.h"



Consumer_Router::Consumer_Router (Peer_Router_Context *prc)
  : Peer_Router (prc)
{
  this->context ()->duplicate ();
}

// Initialize the Router.

int
Consumer_Router::open (void *)
{
  if (this->is_writer ())
    {
      // Set the <Peer_Router_Context> to point back to us so that if
      // any Consumer's "accidentally" send us data we'll be able to
      // handle it by passing it down the stream.
      this->context ()->peer_router (this);

      // Increment the reference count.
      this->context ()->duplicate ();

      // Make this an active object to handle the error cases in a
      // separate thread.  This is mostly just for illustration, i.e.,
      // it's probably overkill to use a thread for this!
      return this->activate (Options::instance ()->t_flags ());
    }
  else // if (this->is_reader ())

    // Nothing to do since this side is primarily used to transmit to
    // Consumers, rather than receive.
    return 0;
}

int
Consumer_Router::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) closing Consumer_Router %s\n"),
              this->is_reader () ? ACE_TEXT ("reader") : ACE_TEXT ("writer")));

  if (this->is_writer ())
    // Inform the thread to shut down.
    this->msg_queue ()->deactivate ();

  // Both writer and reader call <release>, so the context knows when
  // to clean itself up.
  this->context ()->release ();
  return 0;
}

// Handle incoming messages in a separate thread.

int
Consumer_Router::svc (void)
{
  assert (this->is_writer ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) starting svc in Consumer_Router\n")));

  for (ACE_Message_Block *mb = 0;
       this->getq (mb) >= 0;
       )
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) warning: Consumer_Router is ")
                  ACE_TEXT ("forwarding a message to Supplier_Router\n")));

      // Pass this message down to the next Module's writer Task.
      if (this->put_next (mb) == -1)
        ACE_ERROR_RETURN
          ((LM_ERROR,
            ACE_TEXT ("(%t) send_peers failed in Consumer_Router\n")),
           -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) stopping svc in Consumer_Router\n")));
  return 0;
  // Note the implicit ACE_OS::thr_exit() via destructor.
}

// Send a <Message_Block> to the supplier(s).

int
Consumer_Router::put (ACE_Message_Block *mb,
                      ACE_Time_Value *)
{
  // Perform the necessary control operations before passing the
  // message down the stream.

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }

  // If we're the reader then we're responsible for broadcasting
  // messages to Consumers.

  else if (this->is_reader ())
    {
      if (this->context ()->send_peers (mb) == -1)
        ACE_ERROR_RETURN
          ((LM_ERROR,
            ACE_TEXT ("(%t) send_peers failed in Consumer_Router\n")),
           -1);
      else
        return 0;
    }
  else // if (this->is_writer ())
    // Queue up the message to processed by <Consumer_Router::svc>
    // Since we don't expect to be getting many of these messages, we
    // queue them up and run them in a separate thread to avoid taxing
    // the main thread.
    return this->putq (mb);
}

// Return information about the <Consumer_Router>.
#if defined (ACE_WIN32) || !defined (ACE_USES_WCHAR)
#  define FMTSTR  ACE_TEXT ("%s\t %d/%s %s (%s)\n")
#else
#  define FMTSTR  ACE_TEXT ("%ls\t %d/%ls %ls (%ls)\n")
#endif /* ACE_WIN32 || !ACE_USES_WCHAR */

int
Consumer_Router::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr  addr;
  const ACE_TCHAR *module_name = this->name ();

  if (this->context ()->acceptor ().get_local_addr (addr) == -1)
    return -1;

  ACE_OS::sprintf (buf,
                   FMTSTR,
                   module_name,
                   addr.get_port_number (),
                   ACE_TEXT ("tcp"),
                   ACE_TEXT ("# consumer router"),
                   this->is_reader () ? ACE_TEXT ("reader") : ACE_TEXT ("writer"));
  if (*strp == 0 && (*strp = ACE_OS::strdup (module_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, module_name, length);

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (module_name));
}
