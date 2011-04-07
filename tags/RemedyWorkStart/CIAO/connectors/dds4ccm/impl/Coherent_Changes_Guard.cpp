// $Id$

#include "dds4ccm/impl/Coherent_Changes_Guard.h"

CIAO::DDS4CCM::Coherent_Changes_Guard::Coherent_Changes_Guard (
  ::DDS::Publisher_ptr p,
  bool coherent_write) :
    p_ (::DDS::Publisher::_duplicate (p)),
    coherent_write_ (coherent_write)
{
  if (this->coherent_write_)
    {
      p_->begin_coherent_changes ();
    }
}

CIAO::DDS4CCM::Coherent_Changes_Guard::~Coherent_Changes_Guard ()
{
  if (this->coherent_write_)
    {
      this->p_->end_coherent_changes ();
    }
}

