//
// $Id$
//

#include "tao/corba.h"

TAO_Base_Sequence::~TAO_Base_Sequence (void)
{
  if (this->release_)
    this->_deallocate_buffer ();
}
