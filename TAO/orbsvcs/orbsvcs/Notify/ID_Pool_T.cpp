//$Id$
#include "ID_Pool_T.h"

template <class ID_TYPE> void
ID_Pool<ID_TYPE>::put (ID_TYPE /* id */)
{
  // TBD:
}

template <class ID_TYPE> ID_TYPE
ID_Pool<ID_TYPE>::get (void)
{
   return ++id_;
}
