// $Id$

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

AssemblyState
Assembly::get_state()
{
  return this->state_;
}

void
Assembly::build (void)
{
