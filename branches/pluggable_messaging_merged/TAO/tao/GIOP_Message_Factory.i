//$Id$

//
// Utility routines for the accept states
//

ACE_INLINE int
TAO_GIOP_Available_States::check_major (CORBA::Octet &major)
{
  // When we have lots we need to move these definitions to the
  // GIOP_Accept_State.h file. But for now it is ok.. 
  if (major != TAO_DEF_GIOP_MAJOR)
    {
      cout << "Something wrong major"<<endl;
      return -1;
    }
  return 0;
}

ACE_INLINE int
TAO_GIOP_Available_States::check_minor (CORBA::Octet &minor)
{
  // When we have lots we need to move these definitions to the
  // GIOP_Accept_State.h file. But for now it is ok.. 
  if (minor > TAO_DEF_GIOP_MINOR)
    {
      cout << "Something wrong minor"<<endl;
      return -1;
    }
  return 0;
}
