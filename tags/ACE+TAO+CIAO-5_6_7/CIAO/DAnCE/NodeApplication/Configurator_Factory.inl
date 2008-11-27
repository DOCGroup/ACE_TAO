/* -*- C++ -*- */
// $Id$

ACE_INLINE bool 
CIAO::NodeApplication_Options::use_callback ()
{
  return this->use_callback_; 
}

ACE_INLINE bool 
CIAO::NodeApplication_Options::rt_support () 
{
  return this->rt_support_; 
}

ACE_INLINE int 
CIAO::NodeApplication_Options::write_ior_file ()
{ 
  return (this->ior_output_filename_.length () != 0); 
}

ACE_INLINE const char *
CIAO::NodeApplication_Options::ior_output_filename ()
{ 
  return this->ior_output_filename_.c_str (); 
}

ACE_INLINE const char *
CIAO::NodeApplication_Options::callback_ior ()
{ 
  return this->callback_ior_.c_str (); 
}

