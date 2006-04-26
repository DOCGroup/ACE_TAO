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
namespace CIAO
{
  namespace Config_Handlers
  {
    template <typename Seq_Type, typename T>
    class Sequence_Iterator :
      public std::iterator <std::forward_iterator_tag, //iterator type
                            T, // Type pointed to by the iterator
                            CORBA::ULong> // Distance type
    {
    public:
      Sequence_Iterator (void)
        : pos_ (0),
          seq_ (0)
      {
      }

      Sequence_Iterator (const Seq_Type &seq, CORBA::ULong pos = 0)
        : pos_ (pos),
          seq_ (&seq)
      {
      }

      Sequence_Iterator (const Sequence_Iterator &s)
      {
        *this = s;
      }

      Sequence_Iterator& operator= (const Sequence_Iterator &s)
      {
        this->seq_ = s.seq_;
        this->pos_ = s.pos_;
        return *this;
      }

      bool operator== (Sequence_Iterator &s)
      {
        return (this->seq_ == s.seq_) && (this->pos_ == s.pos_);
      }

      bool operator!= (Sequence_Iterator &s)
      {
        return !(*this == s);
      }

      T& operator* (void)
      {
        return (*seq_)[pos_];
      }

      T& operator-> (void)
      {
        return *(*this);
      }

      //  Prefix operator
      Sequence_Iterator& operator++ ()
      {
        ++pos_;
        return *this;
      }

      Sequence_Iterator& operator++ (int)
      {
        Sequence_Iterator ans (*this);
        ++(*this);
        return ans;
      }

    private:
      CORBA::ULong pos_;
      Seq_Type *seq_;
    };

    template <typename Source,
              typename Dest,
              typename Dest_Type,
              void (*Func)(const Source &, Dest_Type &)>
    struct Sequence_Handler
    {
      Sequence_Handler (Dest &dest, CORBA::ULong pos = 0)
         : dest_ (dest),
          pos_ (pos)
      {
      }

      void operator() (const Source &src)
      {
        Func (src, dest_[pos_++]);
      }

    private:
      Dest &dest_;
      CORBA::ULong pos_;
    };

    template <typename Dest, typename Dest_Type>
    struct String_Seq_Handler
    {
      String_Seq_Handler (Dest &dest, CORBA::ULong pos = 0)
         : dest_ (dest),
          pos_ (pos)
      {
      }

      void operator() (const ::XMLSchema::string<ACE_TCHAR>  &src)
      {
        dest_[pos_++] = src.c_str ();
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
