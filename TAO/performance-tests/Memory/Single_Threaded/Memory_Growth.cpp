#include "Memory_Growth.h"

Memory_Growth::Memory_Growth (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Memory_Growth::ping ()
{
  return;
}

void
Memory_Growth::shutdown ()
{
  this->orb_->shutdown (false);
}
