// file      : CCF/IDL2/Parsing/Recovery.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_PARSING_RECOVERY_HPP
#define CCF_IDL2_PARSING_RECOVERY_HPP

#include <memory>

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
      struct RecoveryMethod
      {
        enum Value
        {
          NONE,
          STANDARD,
          BAIL_OUT
        };
      };


      //
      //
      //
      struct DiagnosticType
      {
        enum Value
        {
          BEFORE,
          AFTER,
          NONE
        };
      };


      //
      //
      //
      class Thunk
      {
      public:
        virtual
        ~Thunk () {}

        virtual void
        execute () = 0;

        virtual Thunk*
        clone () const = 0;
      };


      //
      //
      //
      template<typename Object>
      class ThunkImpl : public Thunk
      {
      public:
        typedef void (Object::*Func)();

        ThunkImpl (Object& obj, Func func)
            : obj_(obj),
              func_ (func)
        {
        }

        virtual void
        execute ()
        {
          (obj_.*func_)();
        }

        virtual Thunk*
        clone () const
        {
          return new ThunkImpl<Object> (obj_, func_);
        }

      private:
        Object& obj_;
        Func func_;
      };

      //
      //
      //
      template<typename Object>
      Thunk*
      call_thunk (Object& obj, void (Object::*func)())
      {
        return new ThunkImpl<Object> (obj, func);
      }


      //
      //
      //
      struct RecoveryDescriptor
      {
        RecoveryDescriptor (std::string d,
                            RecoveryMethod::Value recovery,
                            DiagnosticType::Value diagnostic)
            : action_one_ (0),
              action_two_ (0),
              description_ (d),
              recovery_ (recovery),
              diagnostic_ (diagnostic)
        {
        }

        template<typename Object>
        RecoveryDescriptor (std::string d,
                            RecoveryMethod::Value recovery,
                            DiagnosticType::Value diagnostic,
                            Object& obj,
                            void (Object::*action_one)())
            : action_one_ (call_thunk (obj, action_one)),
              action_two_ (0),
              description_ (d),
              recovery_ (recovery),
              diagnostic_ (diagnostic)
        {
        }

        template<typename Object>
        RecoveryDescriptor (std::string d,
                            RecoveryMethod::Value recovery,
                            DiagnosticType::Value diagnostic,
                            Object& obj,
                            void (Object::*action_one)(),
                            void (Object::*action_two)())
            : action_one_ (call_thunk (obj, action_one)),
              action_two_ (call_thunk (obj, action_two)),
              description_ (d),
              recovery_ (recovery),
              diagnostic_ (diagnostic)
        {
        }

        RecoveryDescriptor (RecoveryDescriptor const& rd)
            : action_one_ (rd.action_one_.get () ? rd.action_one_->clone () : 0),
              action_two_ (rd.action_two_.get () ? rd.action_two_->clone () : 0),
              description_ (rd.description_),
              recovery_ (rd.recovery_),
              diagnostic_ (rd.diagnostic_)
        {
        }


        std::auto_ptr<Thunk> action_one_;
        std::auto_ptr<Thunk> action_two_;
        std::string description_;
        RecoveryMethod::Value recovery_;
        DiagnosticType::Value diagnostic_;
      };



      //
      //
      //
      typedef
      parser_error<RecoveryDescriptor, Iterator>
      Error;


      //
      //
      //
      struct Assertion
      {
        assertion<RecoveryDescriptor>
        operator () (RecoveryMethod::Value recovery = RecoveryMethod::NONE)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor ("", recovery, DiagnosticType::NONE));
        }

        assertion<RecoveryDescriptor>
        operator () (std::string d,
                     RecoveryMethod::Value recovery = RecoveryMethod::STANDARD,
                     DiagnosticType::Value diagnostic = DiagnosticType::AFTER)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor (d, recovery, diagnostic));
        }

        template<typename Object>
        assertion<RecoveryDescriptor>
        operator () (std::string d,
                     Object& obj,
                     void (Object::*action_one)(),
                     RecoveryMethod::Value recovery = RecoveryMethod::STANDARD,
                     DiagnosticType::Value diagnostic = DiagnosticType::AFTER)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor (d,
                                recovery,
                                diagnostic,
                                obj,
                                action_one));
        }

        template<typename Object>
        assertion<RecoveryDescriptor>
        operator () (Object& obj,
                     void (Object::*action_one)(),
                     RecoveryMethod::Value recovery = RecoveryMethod::BAIL_OUT)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor ("",
                                recovery,
                                DiagnosticType::NONE,
                                obj,
                                action_one));
        }

        template<typename Object>
        assertion<RecoveryDescriptor>
        operator () (std::string d,
                     Object& obj,
                     void (Object::*action_one)(),
                     void (Object::*action_two)(),
                     RecoveryMethod::Value recovery = RecoveryMethod::STANDARD,
                     DiagnosticType::Value diagnostic = DiagnosticType::AFTER)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor (d,
                                recovery,
                                diagnostic,
                                obj,
                                action_one,
                                action_two));
        }


        template<typename Object>
        assertion<RecoveryDescriptor>
        operator () (Object& obj,
                     void (Object::*action_one)(),
                     void (Object::*action_two)(),
                     RecoveryMethod::Value recovery = RecoveryMethod::BAIL_OUT)
        {
          return assertion<RecoveryDescriptor> (
            RecoveryDescriptor ("",
                                recovery,
                                DiagnosticType::NONE,
                                obj,
                                action_one,
                                action_two));
        }

      };

      typedef
      error_status<>
      RecoveryStatus;

      typedef
      guard<RecoveryDescriptor>
      Guard;
    }
  }
}

#endif  // CCF_IDL2_PARSING_RECOVERY_HPP
