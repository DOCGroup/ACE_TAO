/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Group_Stamp::ACE_CLASSIX_Group_Stamp()
    : ACE_CLASSIX_Group (),
      stamp_            (0)
{
}

ACE_INLINE
ACE_CLASSIX_Group_Stamp::ACE_CLASSIX_Group_Stamp(int theStamp)
    : ACE_CLASSIX_Group ()
{
    this->set_addr(&theStamp);
}

ACE_INLINE
int
ACE_CLASSIX_Group_Stamp::get_stamp() const
{
    return this->stamp_;
	
//    return this->addr_.group_.ui.uiTail;
}

ACE_INLINE
ACE_CLASSIX_Addr::Addr_Type
ACE_CLASSIX_Group_Stamp::is_type(void) const
{
    return ACE_CLASSIX_Addr::STAMP;
}
