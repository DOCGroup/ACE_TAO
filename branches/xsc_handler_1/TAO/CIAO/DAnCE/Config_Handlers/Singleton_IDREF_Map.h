//==============================================================
/**
 *  @file  Singleton_IDREF_Map.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_SINGLETON_IDREF_MAP_H
#define CIAO_CONFIG_HANDLERS_SINGLETON_IDREF_MAP_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/Singleton.h"
#include "ace/SString.h"
// #include "Config_Handlers/XSCRT/XMLSchema.hpp"

namespace CORBA
{
  class String_out;
  class StringSeq;
}

namespace CIAO
{
  namespace Config_Handlers
  {
    /*
     * @class IDREF_Map
     *
     * @brief This class provides a basic definition which is part of
     *  the singleton for mapping arbitrary UUID's to indexes that are
     * used throughout the D+C spec.
     *
     * This class provides NO synchronization whatsoever. It's up to
     * the
     */
    class Config_Handlers_Export IDREF_Map
    {
    public:
      IDREF_Map (void);

      /// The Deployment spec references elements by their position
      /// within their parent sequence.
      /**
       * These two methods allow an element's index to be
       * stored/retrieved.  Map the index <index> of an element to its
       * IDREF <id>.
       */
      bool bind_ref (ACE_CString& id, size_t index);

      /// Retrieve the index of an element with its IDREF <id>.
      /**
       * @\return true if the <id> was found.
       */
      bool find_ref (const ACE_CString& id, size_t val);

      /// Unbind all the elements
      bool unbind_refs (void);

    private:
      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   size_t,
                                   ACE_Null_Mutex> MAP;

      /// The map used to store and look up the indexes of elements
      /// referenced by their index.
      MAP idref_map_;
    };

    typedef ACE_Singleton<IDREF_Map,
                          ACE_Null_Mutex> Singleton_IDREF_Map;
  }
}



#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_BASE_HANDLER_H*/
