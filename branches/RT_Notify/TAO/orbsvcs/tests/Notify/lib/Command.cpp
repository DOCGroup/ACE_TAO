// $Id$

#include "Command.h"

#include "ace/Log_Msg.h"

#if ! defined (__ACE_INLINE__)
#include "Command.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_Command, "$id$")

TAO_NS_Command::TAO_NS_Command (void)
  :next_ (0)
{
}

TAO_NS_Command::~TAO_NS_Command ()
{
}

void
TAO_NS_Command::init (ACE_Arg_Shifter& arg_shifter)
{
  // default: do nothing.
}

void
TAO_NS_Command::next (TAO_NS_Command* command)
{
  this->next_ = command;
}

void
TAO_NS_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG, "Executing command: %s\n", this->get_name ()));

  this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->next_)
    this->next_->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
}
