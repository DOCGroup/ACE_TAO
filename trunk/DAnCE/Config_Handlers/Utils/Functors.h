#ifndef CONFIG_HANDLERS_FUNCTORS_H
#define CONFIG_HANDLERS_FUNCTORS_H

/**
 * @file Functors.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 * Functors useful in the config handlers
 */
#include <iterator>

#include "tao/Basic_Types.h" // For CORBA::ULong
#include "tao/StringSeqC.h"

#include "XMLSchema/Types.hpp"
namespace DAnCE
{
  namespace Config_Handlers
  {
    template <typename Source,
              typename Dest,
              typename Dest_Type,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x630)
              void (Func)(const Source &, Dest_Type &)>
#else
              void (&Func)(const Source &, Dest_Type &)>
#endif
    struct Sequence_Handler
    {
      Sequence_Handler (Dest &dest, CORBA::ULong pos = 0)
         : dest_ (dest),
          pos_ (pos)
      {
      }

      void operator() (const ACE_Refcounted_Auto_Ptr <Source, ACE_Null_Mutex> &src)
      {
        Func (*src, dest_[pos_++]);
      }

    private:
      Dest &dest_;
      CORBA::ULong pos_;
    };

    /*
     * This is a workaround for a GCC bug that for some reason causes
     * functions that appear ONLY in a Sequence_Handler typedef to not
     * be present in the compiled object file.
     * This bug was first observed in GCC 4.02.
     *
     * W: The function we want to be defined
     * X: First argument to the function
     * Y: Second argument to the function
     */
#define SEQ_HAND_GCC_BUG_WORKAROUND(W, X, Y)    \
    while(0) { \
      W (*X, Y[0]);  \
    }


    template <typename Dest, typename Dest_Type>
    struct String_Seq_Handler
    {
      String_Seq_Handler (Dest &dest, CORBA::ULong pos = 0)
         : dest_ (dest),
          pos_ (pos)
      {
      }

      void operator() (const ACE_Refcounted_Auto_Ptr < ::XMLSchema::string<ACE_TCHAR>, ACE_Null_Mutex>  &src)
      {
        dest_[pos_++] = ACE_TEXT_ALWAYS_CHAR (src->c_str ());
      }

    private:
      Dest &dest_;
      CORBA::ULong pos_;
    };

    typedef String_Seq_Handler < ::CORBA::StringSeq,
                                 ::CORBA::String_var > String_Seq_Functor;

  }
}

#endif /* CONFIG_HANDLERS_FUNCTORS_H */
