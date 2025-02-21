#include "DLL_Test_Parent.h"
#include "ace/Log_Msg.h"

Parent::~Parent ()
{
}

void
Parent::test ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parent called\n")));
}

Data::~Data ()
{
}

Base::Base ()
{
}

void
Base::_raise () const
{
  throw *this;
}

Derived::Derived ()
{
}

void
Derived::_raise () const
{
  throw *this;
}

Base*
Derived::_alloc()
{
  return new Derived();
}
