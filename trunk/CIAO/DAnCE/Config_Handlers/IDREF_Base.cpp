// $Id$

#ifndef IDREF_BASE_CPP
#define IDREF_BASE_CPP

#include "DAnCE/Logger/Log_Macros.h"
#include "IDREF_Base.h"
#include <iostream>

namespace DAnCE
  {
    namespace Config_Handlers
    {
      template <typename T>
      void
      IDREF_Base<T>::bind_ref (ACE_TString& id, T value)
      {
        DANCE_TRACE("IDREF_Base<T>::bind_ref");

        int retval =
          idref_map_.bind (id, value);

        pos_map_.bind (value,id);

        if (retval < 0)
          throw Config_Error (id.c_str (),
                              ACE_TEXT ("Failed to bind an IDRef.  This likely indicates a name clash."));
      }

      template <typename T>
      void
      IDREF_Base<T>::bind_next_available (ACE_TString &id)
      {
        // Note:  For this function to compile and work properly,
        // T must have the postincrement operator defined.
        DANCE_TRACE ("IDREF_Base<T>::bind_next_available");
        //      ACE_DEBUG ((LM_DEBUG, "**** Binding %s to %i\n",
        //            id.c_str (),
        //            this->next_));

        int retval = idref_map_.bind (id, this->next_);

        int pos_retval = pos_map_.bind (this->next_, id);

        ++this->next_;

        if (retval < 0 || pos_retval < 0)
          throw Config_Error (id.c_str (),
                              ACE_TEXT ("Failed to bind an IDRef.  This likely indicates a name clash"));

      }

      template <typename T>
      void
      IDREF_Base<T>::find_ref (const ACE_TString& id, T& val)
      {
        DANCE_TRACE("IDREF_Base<T>::find_ref(C_String, T)");

        int retval =
          idref_map_.find (id, val);

        if (retval < 0)
          throw Config_Error (id.c_str (),
                              ACE_TEXT ("Unable to look up an IDRef."));
      }

      template <typename T>
      void
      IDREF_Base<T>::find_ref (const T& value, ACE_TString& id)
      {
        DANCE_TRACE("IDREF_Base<T>::find_ref (T, CString)");

        int retval =
          pos_map_.find (value, id);

        //      ACE_ERROR ((LM_ERROR, "**** Looking up value %i\n",
        //            value));

        if (retval < 0)
          throw Config_Error (ACE_TEXT ("No location information for reverse IDREF lookup"),
                              id.c_str ());
      }

      template <typename T>
      bool
      IDREF_Base<T>::unbind_refs (void)
      {
        DANCE_TRACE("IDREF_Base<T>::unbind_refs");

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
