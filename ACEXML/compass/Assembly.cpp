// $Id$


#if defined (__ACE_INLINE__)
#include "Assembly.inl"
#endif /* __ACE_INLINE__ */

#include "ACEXML/compass/Assembly.h"

using namespace Deployment;

Assembly::Assembly()
  : state_ (AssemblyState::INACTIVE),
    stream_ (0)
{}

Assembly::Assembly (const AssemblyState& state, ACEXML_CharStream* stream)
  : state_ (state),
    stream_ (stream)
{}

Assembly::~Assembly()
{
  this->state_ = Assembly::INACTIVE;
  delete this->stream;
}


void
Assembly::build (void)
  ACE_THROW_SPEC ((CreateFailure))
{}

void
Assembly::tear_down(void)
  ACE_THROW_SPEC ((RemoveFailure))
{}
