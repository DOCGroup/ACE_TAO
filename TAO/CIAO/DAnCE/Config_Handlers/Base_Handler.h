//==============================================================
/**
 *  @file  Base_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_BASE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_BASE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "Config_Handlers/XSCRT/XMLSchema.hpp"

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
     * @class Base_Handler
     *
     * @brief
     */
    class Config_Handlers_Export Base_Handler
    {
    public:
      Base_Handler (void);

      virtual ~Base_Handler (void);

      /// Populates a CORBA string with the provided XMLString.
      void populate_string (const ::XMLSchema::string <char> &str,
                            CORBA::String_out &tofill);

      /// Appends the string provided to the provided StringSeq.
      void populate_string_seq (const ::XMLSchema::string <char> &str,
                                CORBA::StringSeq &tofill);

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
      bool find_ref (ACE_CString& id, size_t val);

    private:
      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   size_t,
                                   ACE_Null_Mutex> IDREF_MAP;

      /// The map used to store and look up the indexes
      /// of elements referenced by their index.
      IDREF_MAP idref_map_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_BASE_HANDLER_H*/
