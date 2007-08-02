// $Id$
#include "NAM_Map.h"
#include "ciao/CIAO_Config.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  NAM_Map::NAM_Map (void)
    : map_ (CIAO_DEFAULT_MAP_SIZE)
  {
  }

  bool
  NAM_Map::is_available (const ACE_CString &str) const
  {
    if (this->map_.find (str) == 0)
      return true;

    return false;
  }

  bool
  NAM_Map::insert_nam (const ACE_CString &str,
                       const PortableServer::ObjectId &oid)
  {
    return (this->map_.bind (str, oid) == 0);
  }

  ::PortableServer::ObjectId
  NAM_Map::get_nam (const ACE_CString &str)
  {
    MAP::ENTRY *entry = 0;

    if (this->map_.find (str, entry) != 0)
      return ::PortableServer::ObjectId ();

    return entry->int_id_.in ();
  }

  bool
  NAM_Map::remove_nam (const PortableServer::ObjectId &oid)
  {
    for (Iterator i = this->map_.begin ();
         i != this->map_.end ();
         ++i)
      {
        if ((*i).int_id_.in() == oid)
          return this->map_.unbind ((*i).ext_id_) == 0;
      }
    return false;
  }
}
