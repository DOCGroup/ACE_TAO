/* -*- C++ -*- */
// $Id$

// Service_Config.i

// This is the primary entry point into the ACE_Service_Config (the
// constructor just handles simple initializations).

ACE_INLINE int
ACE_Service_Config::open (int argc, char *argv[])
{
  ACE_TRACE ("ACE_Service_Config::open");
  ACE_Service_Config::parse_args (argc, argv);
  return ACE_Service_Config::open (argv[0]);
}

// Compare two service descriptors for equality.

ACE_INLINE int
ACE_Static_Svc_Descriptor::operator== (struct ACE_Static_Svc_Descriptor &d) const
{
  return ACE_OS::strcmp (name_, d.name_) == 0;
}
