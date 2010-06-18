// $Id$
#ifndef DNC_DUMP_T_C
#define DNC_DUMP_T_C
#include "DnC_Dump.h"
#include "DnC_Dump_T.h"
#include "Dump_Obj.h"
#include "Logger/Log_Macros.h"
ACE_RCSID (Config_Handlers,
           DnC_Dump_T,
           "$Id$")
  namespace Deployment
  {
    namespace DnC_Dump_T
    {
      // Dumps a reference
      template <typename ROOT, typename REFERENCE, typename DATA_TYPE,
                typename CLASS>
      void
      dump_ref (const char* caption, REFERENCE& ref,
                const char* root, DATA_TYPE CLASS::*data)
      {
        DANCE_DEBUG (1, (LM_DEBUG, "%s%s:\n",
                    Dump_Obj::indent(), caption));

        const CORBA::Any &val = Dump_Obj::desc (root);

        ROOT *t = 0;

        val >>= t;

        const ROOT &root_obj = *t;

        int value = ref;
        DANCE_DEBUG (1, (LM_DEBUG, "%s%s.name: %s\n",
                    Dump_Obj::indent(), caption,
                    (root_obj.*data)[value].name.in()));
      }

      // Dumps a reference sequence
      template <typename ROOT, typename SEQUENCE, typename DATA_TYPE,
                typename CLASS>
      void
      dump_ref_seq (const char* caption, SEQUENCE& seq,
                    const char* root, DATA_TYPE CLASS::*data)
      {
        DANCE_DEBUG (1, (LM_DEBUG,
                    "%s%s:\n",
                    Dump_Obj::indent(), caption));

        CORBA::Any val;
        val <<= root;

        for (size_t i = 0; i < seq.length(); ++i)
          {
            const CORBA::Any &val = Dump_Obj::desc (root);

            ROOT *t = 0;

            val >>= t;

            const ROOT &root_obj = *t;
            int value = seq[i];
            DANCE_DEBUG (1, (LM_DEBUG, "%s%s[%d].name: %s\n",
                        Dump_Obj::indent(), caption, i,
                        (root_obj.*data)[value].name.in()));
          }
      }

      // Dumps a sequence
      template <typename SEQUENCE>
      void
      dump_sequence (const char* caption, const SEQUENCE &seq)
      {
        CORBA::ULong size = seq.length ();

        if (size != 0)
          {
            Dump_Obj dump_obj(caption);

            for (CORBA::ULong i = 0; i < size; ++i)
              {
                DANCE_DEBUG (1, (LM_DEBUG, "%s%s %d:\n", Dump_Obj::indent(),
                            caption, i));
                DnC_Dump::dump (seq[i]);
              }
          }
      }
    }
  }
#endif /* DNC_DUMP_C */
