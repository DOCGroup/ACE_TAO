// $Id$
#include "Singleton_IDREF_Map.h"
#include "tao/CORBA_String.h"
#include "tao/StringSeqC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    IDREF_Map::IDREF_Map (void)
      // @@ MAJO, shodl be configured with some decent value.
      : idref_map_ ()
    {
    }

    bool
    IDREF_Map::bind_ref (ACE_CString& id, size_t index)
    {
      int retval =
        this->idref_map_.bind (id, index);

      if (retval < 0)
        return false;

      return true;
    }

    bool
    IDREF_Map::find_ref (const ACE_CString& id, size_t val)
    {
      int retval =
        this->idref_map_.find (id, val);

      if (retval < 0)
        return false;

      return true;
    }

    bool
    IDREF_Map::unbind_refs (void)
    {
      int retval =
        this->idref_map_.unbind_all ();

      if (retval < 0)
        return false;

      return true;
    }

  }
}
