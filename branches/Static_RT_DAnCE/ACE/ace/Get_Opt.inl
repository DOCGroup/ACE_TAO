// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
ACE_Get_Opt::ACE_Get_Opt_Long_Option::operator < (const ACE_Get_Opt_Long_Option &rhs)
{
  return this->name_ < rhs.name_;
}

ACE_INLINE int
ACE_Get_Opt::argc (void) const
{
  return this->argc_;
}

ACE_INLINE ACE_TCHAR **
ACE_Get_Opt::argv (void) const
{
  return this->argv_;
}

ACE_INLINE ACE_TCHAR*
ACE_Get_Opt::opt_arg (void) const
{
  return this->optarg;
}

ACE_INLINE int
ACE_Get_Opt::opt_opt (void)
{
  return this->optopt_;
}

ACE_INLINE int &
ACE_Get_Opt::opt_ind (void)
{
  return this->optind;
}

ACE_END_VERSIONED_NAMESPACE_DECL
