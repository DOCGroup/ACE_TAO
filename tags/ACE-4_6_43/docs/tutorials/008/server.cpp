// $Id$

/* Our datagram server will, of course, need to create a datagram.
   We'll also need an address object so that we know where to listen.  */
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"

/* Use the typical TCP/IP port address for receiving datagrams.  */
static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int 
main (int, char**)
{
  /* This is where we'll listen for datagrams coming from the clients.
    We'll give this address to the open() method below to enable the
    listener.  */
  ACE_INET_Addr local (PORT);

  /* A simply constructed datagram that we'll listen with.  */
  ACE_SOCK_Dgram dgram;

  /* Like most ACE objects, the datagram has to be opened before it
    can be uses.  Of course, -1 on failure.

    A datagram will fail to open if there is already a datagram
    listening at the port we've chosen.  It *is* OK to open a datagram
    at a port where there is an ACE_SOCK_Stream though.  This is
    because datagrams are UDP and SOCK_Stream is TCP and the two don't
    cross paths.  */
  if (dgram.open (local) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  /* Create a simple buffer to receive the data.  You generally need
    to provide a buffer big enough for the largest datagram you expect
    to receive.  Some platforms will let you read a little and then
    some more later but other platforms will throw out whatever part
    of the datagram you don't get with the first read.  (This is on a
    per-datagram basis BTW.)  The theoretical limit on a datagram is
    about 64k.  The realistic limit (because of routers & such) is
    much smaller.  Choose your buffer size based on your application's
    needs.  */
  char buf[BUFSIZ];

  /* Unlike ACE_SOCK_Stream, datagrams are unconnected.  That is,
    there is no "virtual circuit" between server and client.  Because
    of this, the server has to provide a placeholder for the OS to
    fill in the source (client) address information on the recv.  You
    can initialize this INET_Addr to anything, it will be overwritten
    when the data arrives. */
  ACE_INET_Addr remote;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) starting up server daemon\n"));

  /* Receive datagrams as long as we're able.  */
  while (dgram.recv (buf,
                     sizeof (buf),
                     remote) != -1)
    {
      /* Display a brief message about our progress.  Notice how we
        use the 'remote' object to display the address of the client.
        With an ACE_SOCK_Stream we used get_remote_addr() to get the
        address the socket is connected to.  Because datagrams are
        unconnected, we use the addr object provided to recv().  */
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Data (%s) from client (%s)\n",
                  buf,
                  remote.get_host_name ()));

      /* To respond to the client's query, we have to become a client
        ourselves.  To do so, we need an anonymous local address from
        which we'll send the response and a datagram in which to send
        it.  (An anonymous address is simply one where we let the OS
        choose a port for us.  We really don't care what it is.  */
      ACE_INET_Addr local ((u_short) 0);
      ACE_SOCK_Dgram client;

      /* Open up our response datagram as always.  */
      if (client.open (local) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "client open"),
                            -1);
          return 0;
        }

      /* Build a witty response...  */
      sprintf (buf,
               "I am here");

      /* and send it to the client.  Notice the symmetry with the
        recv() method.  Again, the unconnected nature of datagrams
        forces us to specify an address object with each read/write
        operation.  In the case of read (recv()) that's where the OS
        stuffs the address of the datagram sender.  In the case of
        write (send()) that we're doing here, the address is where we
        want the network to deliver the data.

        Of course, we're assuming that the client will be listening
        for our reply...  */
      if (client.send (buf,
                       ACE_OS::strlen (buf) + 1,
                       remote) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "send"),
                          -1);
    }

  return 0;
}
