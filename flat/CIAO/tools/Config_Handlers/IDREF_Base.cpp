// $Id$

#ifndef IDREF_BASE_CPP
#define IDREF_BASE_CPP

#include "ciao/CIAO_common.h"
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
      void
      IDREF_Base<T>::bind_ref (ACE_CString& id, T value)
      {
        CIAO_TRACE("IDREF_Base<T>::bind_ref");

        int retval =
          idref_map_.bind (id, value);

        pos_map_.bind (value,id);

        if (retval < 0)
          throw Config_Error (id.c_str (),
                              "Failed to bind an IDRef.  This likely indicates a name clash.");
      }

      template <typename T>
      void
      IDREF_Base<T>::bind_next_available (ACE_CString &id)
      {
        // Note:  For this function to compile and work properly,
        // T must have the postincrement operator defined.
        CIAO_TRACE ("IDREF_Base<T>::bind_next_available");
        //      ACE_DEBUG ((LM_DEBUG, "**** Binding %s to %i\n",
        //            id.c_str (),
        //            this->next_));

        int retval = idref_map_.bind (id, this->next_);

        int pos_retval = pos_map_.bind (this->next_, id);

        ++this->next_;

        if (retval < 0 || pos_retval < 0)
          throw Config_Error (id.c_str (),
                              "Failed to bind an IDRef.  This likely indicates a name clash");

      }

      template <typename T>
      void
      IDREF_Base<T>::find_ref (const ACE_CString& id, T& val)
      {
        CIAO_TRACE("IDREF_Base<T>::find_ref(C_String, T)");

        int retval =
          idref_map_.find (id, val);

        if (retval < 0)
          throw Config_Error (id.c_str (),
                              "Unable to look up an IDRef.");
      }

      template <typename T>
      void
      IDREF_Base<T>::find_ref (const T& value, ACE_CString& id)
      {
        CIAO_TRACE("IDREF_Base<T>::find_ref (T, CString)");

        int retval =
          pos_map_.find (value, id);

        //      ACE_ERROR ((LM_ERROR, "**** Looking up value %i\n",
        //            value));

        if (retval < 0)
          throw Config_Error ("No location information for reverse IDREF lookup",
                              id.c_str ());
      }

      template <typename T>
      bool
      IDREF_Base<T>::unbind_refs (void)
      {
        CIAO_TRACE("IDREF_Base<T>::unbind_refs");

        int retval =
          idref_map_.unbind_all ();

        pos_map_.unbind_all ();

        this->next_ = 0;

        if (retval < 0)
          return false;

        return true;
      }
    }

  }

#endif /* IDREF_BASE_CPP */
