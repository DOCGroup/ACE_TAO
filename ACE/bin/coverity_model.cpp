// $Id$

namespace CORBA
{
  typedef bool Boolean;
  
  CORBA::Boolean is_nil (void* x)
  {
    return x == 0;
  }
}
