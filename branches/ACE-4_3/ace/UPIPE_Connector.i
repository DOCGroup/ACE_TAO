/* -*- C++ -*- */
// $Id$

// UPIPE_Connector.i

// Creates a Local ACE_UPIPE. 

inline
ACE_UPIPE_Connector::ACE_UPIPE_Connector (ACE_UPIPE_Stream &new_stream, 
					  const ACE_UPIPE_Addr &addr,
					  ACE_Time_Value *timeout,
					  const ACE_Addr &local_sap,
					  int reuse_addr,
					  int flags,
					  int perms)
{
  ACE_TRACE ("ACE_UPIPE_Connector::ACE_UPIPE_Connector");
  if (this->connect (new_stream, addr, timeout, local_sap, 
		     reuse_addr, flags, perms) == -1
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIMEDOUT))
    ACE_ERROR ((LM_ERROR, "address %s, %p\n", 
	       addr.get_path_name (), "ACE_UPIPE_Connector"));
}





