// @(#) $Id$

#include "Default_Acceptor_Filter.h"
#include "tao/Pluggable.h"

ACE_RCSID(tao, Default_Acceptor_Filter, "$Id$")

TAO_Default_Acceptor_Filter::TAO_Default_Acceptor_Filter (void)
{
}

int
TAO_Default_Acceptor_Filter::fill_mprofile (const TAO_ObjectKey &object_key,
                                            TAO_MProfile &mprofile,
                                            TAO_Acceptor **acceptors_begin,
                                            TAO_Acceptor **acceptors_end)
{
  for (TAO_Acceptor** acceptor = acceptors_begin;
       acceptor != acceptors_end;
       ++acceptor)
    {
      if ((*acceptor)->create_mprofile (object_key,
                                        mprofile,
                                        0 /* one endpoint per profile */)
          == -1)
        return -1;
    }

  return 0;
}
