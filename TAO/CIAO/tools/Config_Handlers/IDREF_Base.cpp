// $Id$
#ifndef IDREF_BASE_CPP
#define IDREF_BASE_CPP

#include "IDREF_Base.h"
#include <iostream>


ACE_RCSID (DAnCE,
           IDREF_Base,
           "$Id$")

 
namespace CIAO
{
  namespace Config_Handlers
  {
    template <typename T>
    bool
    IDREF_Base<T>::bind_ref (ACE_CString& id, T value)
    {
      int retval =
        idref_map_.bind (id, value);

      pos_map_.bind (value,id);

      if (retval < 0)
        return false;

      return true;
    }

    template <typename T>
    bool
    IDREF_Base<T>::find_ref (const ACE_CString& id, T& val)
    {
      int retval =
        idref_map_.find (id, val);

      if (retval < 0)
        return false;

      return true;
    }

    template <typename T>
    bool
    IDREF_Base<T>::find_ref (const T& value, ACE_CString& id)
    {
      int retval =
        pos_map_.find (value, id);

      if (retval < 0)
        return false;

      return true;
    }

    template <typename T>
    bool
    IDREF_Base<T>::unbind_refs (void)
    {
      int retval =
        idref_map_.unbind_all ();
 
      pos_map_.unbind_all ();
   
      if (retval < 0)
        return false;

      return true;
    }
}

}

#endif /* IDREF_BASE_CPP */
