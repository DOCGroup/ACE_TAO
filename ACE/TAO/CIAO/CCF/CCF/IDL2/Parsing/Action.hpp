// file      : CCF/IDL2/Parsing/Action.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_PARSING_ACTION_HPP
#define CCF_IDL2_PARSING_ACTION_HPP

#include "CCF/IDL2/Parsing/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Parsing
    {
      //
      //
      //
      template <typename Obj>
      class ActionExecutor
      {
      public:

        typedef
        void (Obj::*SemanticAction)(Iterator, Iterator) const;

        ActionExecutor (Obj const* obj, SemanticAction action)
            : obj_ (obj), action_ (action)
        {
        }

        void operator () (Iterator begin, Iterator end) const
        {
          (obj_->*action_) (begin, end);
        }

      private:
        Obj const* obj_;
        SemanticAction action_;
      };


      //
      //
      //
      template <typename Obj>
      class NoArgAction
      {
      public:

        typedef
        void (Obj::*Action)();

        NoArgAction (Obj& obj, Action action)
            : obj_ (obj), action_ (action)
        {
        }

        void operator () (Iterator, Iterator) const
        {
          (obj_.*action_) ();
        }

      private:
        Obj& obj_;
        Action action_;
      };


      //@@ Should prbably use Type instead of TypePtr
      //
      //
      //
      template <typename TypePtr, typename Obj>
      class OneArgAction
      {
      public:

        typedef
        void (Obj::*Action)(TypePtr const&);

        OneArgAction (Obj& obj, Action action)
            : obj_ (obj), action_ (action)
        {
        }

        void operator () (Iterator begin, Iterator end) const
        {
          if (end - begin != 1 )
          {
            //@@ throw
          }

          //@@ error handling if the strict_cast fails
          (obj_.*action_) (
            ReferenceCounting::strict_cast<typename TypePtr::Type> (*begin));
        }

      private:
        Obj& obj_;
        Action action_;
      };


      //
      //
      //
      template <typename Arg1, typename Arg2, typename Obj>
      class TwoArgAction
      {
      public:

        typedef
        void (Obj::*Action)(Arg1 const&, Arg2 const&);

        TwoArgAction (Obj& obj, Action action)
            : obj_ (obj), action_ (action)
        {
        }

        void operator () (Iterator begin, Iterator end) const
        {
          if (end - begin != 2 )
          {
            //@@ throw
          }

          //@@ error handling if strict_cast fails
          (obj_.*action_) (
            ReferenceCounting::strict_cast<typename Arg1::Type> (*begin),
            ReferenceCounting::strict_cast<typename Arg2::Type> (*(begin + 1)));
        }

      private:
        Obj& obj_;
        Action action_;
      };
    }
  }
}

#endif  // CCF_IDL2_PARSING_ACTION_HPP
