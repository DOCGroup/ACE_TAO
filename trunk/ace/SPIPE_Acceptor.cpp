// SPIPE_Acceptor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SPIPE_Acceptor.h"

ACE_SPIPE_Acceptor::ACE_SPIPE_Acceptor (void)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::ACE_SPIPE_Acceptor");
}

int
ACE_SPIPE_Acceptor::remove (void)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::remove");
  int result = this->close ();
#if defined (ACE_HAS_STREAM_PIPES)
  // Remove the underlying file.
  return ACE_OS::unlink (this->local_addr_.get_path_name ()) == -1 
    || result == -1 ? -1 : 0;
#else
  return 0;
#endif
}

ACE_ALLOC_HOOK_DEFINE(ACE_SPIPE_Acceptor)

void
ACE_SPIPE_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::dump");
}

// General purpose routine for performing server ACE_SPIPE creation.

int
ACE_SPIPE_Acceptor::open (const ACE_SPIPE_Addr &local_sap, 
			  int /* reuse_addr */,
			  int perms)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::open");

  this->local_addr_ = local_sap;
  this->set_handle (ACE_INVALID_HANDLE);

  return this->create_new_instance (perms);
}

int
ACE_SPIPE_Acceptor::create_new_instance (int perms)
{
#if defined (ACE_HAS_STREAM_PIPES)
  ACE_HANDLE handle;
  ACE_HANDLE spipe[2];
  
  handle = ACE_OS::creat (this->local_addr_.get_path_name (), perms);

  if (handle == ACE_INVALID_HANDLE)
    return -1;
  else if (ACE_OS::close (handle) == -1)
    return -1;
  else if (ACE_OS::pipe (spipe) == -1)
    return -1;
  else if (ACE_OS::ioctl (spipe[0], I_PUSH, "connld") == -1)
    return -1;
  else if (ACE_OS::fattach (spipe[0], this->local_addr_.get_path_name ()) == -1)
    return -1;

  this->set_handle (spipe[1]);
  return 0;
#elif defined (ACE_WIN32)
  // Create a new instance of the Named Pipe (WIN32).  A new instance
  // of the named pipe must be created for every client process. If an
  // instance of the named pipe that is already connected to a client
  // process is reused with a new client process, ::ConnectNamedPipe()
  // would fail.

  ACE_TRACE ("ACE_SPIPE_Acceptor::create_new_instance");

  // Create a new instance of the named pipe
  ACE_HANDLE handle = ::CreateNamedPipe (this->local_addr_.get_path_name (),
					 PIPE_ACCESS_DUPLEX |
					 FILE_FLAG_OVERLAPPED,
					 PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
					 MAX_PIPE_INSTANCES,
					 1024*10,
					 1024*10,
					 ACE_DEFAULT_TIMEOUT,
					 NULL);
  if (handle == ACE_INVALID_HANDLE)
    return -1;
  else
    this->set_handle (handle);
  return 0;
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

int
ACE_SPIPE_Acceptor::close (void)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::close");

  // This behavior is shared by UNIX and Win32...
  int result = this->ACE_SPIPE::close ();
  this->set_handle (ACE_INVALID_HANDLE);

#if defined (ACE_HAS_STREAM_PIPES)
  ACE_OS::fdetach (this->local_addr_.get_path_name ());
#endif /* ACE_HAS_STREAM_PIPES */
  return result;
}

ACE_SPIPE_Acceptor::ACE_SPIPE_Acceptor (const ACE_SPIPE_Addr &local_sap, 
					int reuse_addr,
					int perms)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::ACE_SPIPE_Acceptor");

  if (this->open (local_sap, reuse_addr, perms) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SPIPE_Acceptor"));
}

// General purpose routine for accepting new connections.

int
ACE_SPIPE_Acceptor::accept (ACE_SPIPE_Stream &new_io, 
			    ACE_SPIPE_Addr *remote_addr,
			    ACE_Time_Value *timeout, 
			    int restart)
{
  ACE_TRACE ("ACE_SPIPE_Acceptor::accept");
#if defined (ACE_HAS_STREAM_PIPES)
  strrecvfd r_handle;

  // Note that if THIS->MILLI_SECOND_DELAY == -1 we block on
  // ACE_OS::ioctl(). Otherwise, we will wait for the desired number
  // of milli seconds using ACE_OS::poll.

  if (timeout != 0 && 
      ACE::handle_timed_accept (this->get_handle (), timeout, restart) == -1)
    return -1;
  else if (ACE_OS::ioctl (this->get_handle (), I_RECVFD, &r_handle) == -1)
    return -1;

  new_io.set_handle (r_handle.fd);
  new_io.local_addr_ = this->local_addr_;
  new_io.remote_addr_.set_size (sizeof r_handle.gid + sizeof r_handle.uid);
  new_io.remote_addr_.group_id (r_handle.gid);
  new_io.remote_addr_.user_id (r_handle.uid);

  // This is for compatibility with ACE_SOCK_Acceptor and
  // ACE_TLI_Acceptor.
  if (remote_addr != 0)
    *remote_addr = new_io.remote_addr_;

  return 0;
#elif defined (ACE_WIN32)
  // Check to see if we have a valid pipe
  if (this->get_handle () == ACE_INVALID_HANDLE)
    return -1;

  // Accept connection on the current instance of the named pipe Note
  // that the connection is done synchronously, by specifying a NULL
  // OVERLAPPED structure pointer.

  int attempts = 0;

  for (;;)
    {
      attempts++;

      if (::ConnectNamedPipe (this->get_handle (), NULL) != 0)
	break;

      DWORD error = ::GetLastError ();

      if (error == ERROR_NO_DATA)
	{
	  // A client connected and disconnected in the interval
	  // between the creation of the named pipe instance and the
	  // call to ::ConnectNamedPipe ().  The named pipe handle
	  // must be disconnected from this fleeting client before it
	  // can be reconnected to new client.

	  if (attempts > MAX_ACCEPT_ATTEMPTS)
	    return -1;
	  else if (::DisconnectNamedPipe (this->get_handle ()) != 0)
	    return -1;
	  // Loop and make another connection attempt.
	}
      else if (error == ERROR_PIPE_CONNECTED)
	// A client connected in the interval between the creation of
	// the named pipe instance and the call to ::ConnectNamedPipe
	// ().  However, the connection is alright.
	break;
      else // Unrecoverable connection failure, so let's bail out...
	return -1;
    }

  new_io.set_handle (this->get_handle ());
  new_io.local_addr_ = this->local_addr_;

  // Create a new instance of the pipe for the next connection.
  return this->create_new_instance ();
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}
