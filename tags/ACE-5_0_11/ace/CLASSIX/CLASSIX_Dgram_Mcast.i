/* -*- C++ -*- */
// $Id$
//
// ace/CLASSIX_Dgram_Mcast.i
/* ------------------------------------------------------------------------- */
ACE_INLINE
ACE_CLASSIX_Dgram_Mcast::ACE_CLASSIX_Dgram_Mcast(void)
    : ACE_CLASSIX_Stream()
{
}

ACE_INLINE
ACE_CLASSIX_Dgram_Mcast::ACE_CLASSIX_Dgram_Mcast (int theStamp,
				const ACE_CLASSIX_Port& thePort)
    : ACE_CLASSIX_Stream()
{
    this->set_saps(theStamp, thePort);
}


/* ------------------------------------------------------------------------- */
