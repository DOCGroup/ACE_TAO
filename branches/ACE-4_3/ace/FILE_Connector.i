/* -*- C++ -*- */
// $Id$

// FILE_Connector.i

// Creates a Local ACE_FILE. 

inline
ACE_FILE_Connector::ACE_FILE_Connector (ACE_FILE_IO &new_io, 
					const ACE_FILE_Addr &remote_sap,
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
					int reuse_addr,
					int flags,
					int perms)
{
  ACE_TRACE ("ACE_FILE_Connector::ACE_FILE_Connector");
  if (this->connect (new_io, remote_sap, timeout, local_sap, 
		     reuse_addr, flags, perms) == ACE_IO_SAP::INVALID_HANDLE
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIMEDOUT))
    ACE_ERROR ((LM_ERROR, "address %s, %p\n", 
	       remote_sap.get_path_name (), "ACE_FILE_IO"));
}
