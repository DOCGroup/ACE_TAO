/* -*- C++ -*- */
// $Id$

#include "CLASSIX_CO_Acceptor.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "CLASSIX_CO_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/Synch.h"

ACE_ALLOC_HOOK_DEFINE (ACE_CLASSIX_CO_Acceptor)

// General purpose routine for performing server  creation.

ACE_CLASSIX_CO_Acceptor::ACE_CLASSIX_CO_Acceptor (const ACE_Addr &local_sap, 
						  int reuse_addr, 
						  int protocol_family,
						  int backlog, 
						  int protocol,
						  ACE_Reactor* theReactor)
  : port_ (0),
    reactor_ (theReactor)
{
  ACE_TRACE ("ACE_CLASSIX_CO_Acceptor::ACE_CLASSIX_CO_Acceptor");
  if (this->open (local_sap, reuse_addr, protocol_family, 
		  backlog, protocol, theReactor) == -1)
    ACE_ERROR ((LM_ERROR, 
                ASYS_TEXT ("%p\n"), 
                ASYS_TEXT ("ACE_CLASSIX_CO_Acceptor")));
}

// General purpose routine for performing server creation.

int
ACE_CLASSIX_CO_Acceptor::open (const ACE_Addr & theLocalSap, 
			       int /* reuse_addr */,
			       int theProtocolFamily, 
			       int /* backlog */, 
			       int /* protocol */,
			       ACE_Reactor* theReactor)
{
  // make sure that the client requests for a "CLASSIX" family.

  if (theProtocolFamily != ACE_CLASSIX_Addr::AF_CLASSIX)
    return -1;
  
  // Setup local address

  int error = 0;
  if (theLocalSap == ACE_Addr::sap_any)
    {
      this->port_ = new ACE_CLASSIX_Port_Core ();
      ACE_ASSERT (this->port_);
      error = this->addr_.set (*this->port_);
    }
  else
    error = this->addr_.set (theLocalSap);

  if (error != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Acceptor::open ()::"
                      "Cannot setup address\n"), -1);

  // Make the port "selectable"

  if (this->addr_.selectable () != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Acceptor::open ()::"
                      "Cannot enable port\n"), -1);

  this->reactor_ = theReactor;
  return 0;
}
    
// General purpose routine for accepting new connections. 

int
ACE_CLASSIX_CO_Acceptor::accept (ACE_CLASSIX_Stream &new_stream, 
				 ACE_Addr *remote_addr, 
				 ACE_Time_Value* /* timeout */, 
				 int /* restart */,
				 int /* reset_new_handle */) const
{
  ACE_TRACE ("ACE_CLASSIX_CO_Acceptor::accept");

  // Get peer's address info

  size_t size = 0;
  if (this->reactor_->current_info (this->get_handle (), size) 
      == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Acceptor:.accept ()::"
                       "Cannot get peer addr\n"), -1);

  ACE_CLASSIX_CO_Acceptor::Peer_Union data;
  ACE_CLASSIX_Msg rmsg (&data, sizeof (ACE_CLASSIX_CO_Acceptor::Peer_Union));
  if (size != sizeof (ACE_CLASSIX_CO_Acceptor::Peer_Union) 
      || ::ipcGetData (rmsg.get ()) != size)
    ACE_ERROR_RETURN ((LM_ERROR, "%t"
                       "ACE_CLASSIX_CO_Acceptor::accept::ipcGetData (%d)\n",
                       size),
                      -1);

  // Set up Peer SAP

  int error = 0;
  switch (data.type)
    {
    case ACE_CLASSIX_Addr::PORT:
      error = new_stream.set_peer_sap (data.choose.port.peer);
      break;

    case ACE_CLASSIX_Addr::GROUP:
      error = new_stream.set_peer_sap (data.choose.group.peer);
      break;

    case ACE_CLASSIX_Addr::STAMP:
      error = new_stream.set_peer_sap (data.choose.stamp.peer);
      break;

    case ACE_CLASSIX_Addr::DYNAMIC:
      error = new_stream.set_peer_sap (data.choose.group.peer);
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Accept::accept ()::"
                         "Invalid peer address\n"), -1);
    }

  if (error != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Accept::accept ()::"
                       "cannot set peer address\n"), -1);

  // Set up local SAP & tell the peer

  data.type = ACE_CLASSIX_Addr::UNDEFINED;

  if (remote_addr != 0)
    {
      if (remote_addr->get_type () == ACE_CLASSIX_Addr::AF_CLASSIX &&
          ((ACE_CLASSIX_Addr*) remote_addr)->is_type == 
	  ACE_CLASSIX_Addr::PORT)
        {
	  new_stream.get_local_sap ().set (*remote_addr);
	  data.type = ACE_CLASSIX_Addr::PORT;
        }
    }
  else
    {
      ACE_CLASSIX_Port_Core *port = new ACE_CLASSIX_Port_Core ();
      if (new_stream.set_local_port (port) == 0)
        data.type = ACE_CLASSIX_Addr::PORT;;
    }

  data.choose.port.peer =
    new_stream.get_local_sap ().get_addr ().get_id ();

  ACE_CLASSIX_Msg smsg (&data, sizeof (ACE_CLASSIX_CO_Acceptor::Peer_Union));

  if (::ipcReply (smsg.get ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_CLASSIX_CO_Accept::accept ()::"
                       "cannot deliver server address to peer\n"), -1);

  // Make new_stream's local SAP selectable.
  if (new_stream.selectable () != 0)
    return -1;

  return 0;
}

int
ACE_CLASSIX_CO_Acceptor::close (void)
{
  ACE_TRACE ("ACE_CLASSIX_CO_Acceptor::close");
  int result = 0;

  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      delete this->port_;
      this->port_ = 0;
      ((ACE_CLASSIX_Port) this->addr_.get_addr ()).clear ();
    }
  return 0;
}


int
ACE_CLASSIX_CO_Acceptor::get_local_addr (ACE_Addr &theAddr) const
{
  int size = theAddr.get_size ();
  if (size < this->addr_.get_addr ().get_size ())
    return -1;

  return this->addr_.get_addr (theAddr);
}

void
ACE_CLASSIX_CO_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_CLASSIX_CO_Acceptor::dump");
}
