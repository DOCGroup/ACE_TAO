/* -*- C++ -*- */
// $Id$

// Get_Opt.i

ACE_INLINE int 
ACE_Get_Opt::ACE_Get_Opt_Long_Option::operator < (const ACE_Get_Opt_Long_Option &rhs)
{
  return this->name_ < rhs.name_;
}

ACE_INLINE ACE_TCHAR*
ACE_Get_Opt::opt_arg (void) const 
{ 
  return this->opt_arg_; 
}

ACE_INLINE int &
ACE_Get_Opt::opt_ind (void)
{ 
  return this->opt_ind_; 
}

ACE_INLINE const ACE_TCHAR *
ACE_Get_Opt::optstring (void) const 
{ 
  return this->optstring_.c_str ();
}
