// $Id$

#include "IDREF_Base.h"
#include <iostream>


ACE_RCSID (DAnCE,
           IDREF_Base,
           "$Id$")

 
namespace CIAO
{
  namespace Config_Handlers
  {

    bool
    IDREF_Base::bind_ref (ACE_CString& id, size_t index)
    {
      int retval =
        idref_map_.bind (id, index);

      pos_map_.bind (index,id);

      if (retval < 0)
        return false;

      return true;
    }

    bool
    IDREF_Base::find_ref (const ACE_CString& id, size_t& val)
    {
      int retval =
        idref_map_.find (id, val);

      if (retval < 0)
        return false;

      return true;
    }
    
    bool
    IDREF_Base::find_ref (const size_t id, ACE_CString& val)
    {
      int retval =
        pos_map_.find (id,val);

      if (retval < 0)
        return false;

      return true;
    }

    bool
    IDREF_Base::unbind_refs (void)
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

