/* -*- C++ -*- */
// $Id$

// MEM_Connector.i

// Establish a connection.

ASYS_INLINE ACE_MEM_SAP::MALLOC_OPTIONS &
ACE_MEM_Connector::malloc_options (void)
{
  return this->malloc_options_;
}
