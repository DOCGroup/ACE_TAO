// $Id$

#include "Operation_Table_Parameters.h"

ACE_RCSID(PortableServer,
          Operation_Table_Parameters,
          "$Id$")

TAO_Operation_Table_Parameters::TAO_Operation_Table_Parameters (void)
  : strategy_ (0),
    type_ (TAO_Operation_Table_Parameters::TAO_DYNAMIC_HASH) // default
{
}

TAO_Operation_Table_Parameters::~TAO_Operation_Table_Parameters (void)
{
}

void
TAO_Operation_Table_Parameters::lookup_strategy (TAO_Operation_Table_Parameters::DEMUX_STRATEGY s)
{
  this->type_ = s;
}

// get the lookup type
TAO_Operation_Table_Parameters::DEMUX_STRATEGY
TAO_Operation_Table_Parameters::lookup_strategy (void) const
{
  return this->type_;
}

// set the concrete strategy
void
TAO_Operation_Table_Parameters::concrete_strategy (TAO_Operation_Table *ot)
{
  this->strategy_ = ot;
}

// return the concrete strategy
TAO_Operation_Table*
TAO_Operation_Table_Parameters::concrete_strategy (void)
{
  return this->strategy_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Singleton<TAO_Operation_Table_Parameters,
                             TAO_SYNCH_RECURSIVE_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Singleton<TAO_Operation_Table_Parameters, TAO_SYNCH_RECURSIVE_MUTEX>

#elif defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)

template TAO_Singleton<TAO_Operation_Table_Parameters, TAO_SYNCH_RECURSIVE_MUTEX>* TAO_Singleton<TAO_Operation_Table_Parameters, TAO_SYNCH_RECURSIVE_MUTEX >::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
