// $Id$

namespace CORBA
{
  typedef bool Boolean;

  static CORBA::Boolean is_nil (void* x)
  {
    return x == 0;
  }
}
