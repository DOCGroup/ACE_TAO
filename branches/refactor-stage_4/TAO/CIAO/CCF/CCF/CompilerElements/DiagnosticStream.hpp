// file      : CCF/Runtime/DiagnosticStream.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_RUNTIME_DIAGNOSTIC_DIAGNOSTIC_STREAM_H
#define CCF_RUNTIME_DIAGNOSTIC_DIAGNOSTIC_STREAM_H

#include "MSVC_Pragmas.hpp"

#include <string>
#include <sstream>

#include "CCF/CompilerElements/ExH.hpp"

namespace CCF
{
  namespace Runtime
  {
    namespace Diagnostic
    {
      class Message
      {
      public:
        virtual
        ~Message () throw ();

        virtual std::string
        text () const throw (ExH::System::Exception) = 0;
      };

      class Stream
      {
      public:
        Stream () throw ();
        virtual
        ~Stream () throw ();

      public:
        Stream& operator<< (Message const& msg)
          throw (ExH::System::Exception);

      private:
        // Copy semanic is not supported.
        Stream (Stream const&) throw ();
        Stream& operator= (Stream const&) throw ();
      };

      extern Stream dout;

      //
      // Concrete Message classes.
      //

      template <typename Category>
      class MessageImpl : public virtual Message,
                          public virtual Category
      {
      public:
        MessageImpl (char const* subject, char const* message)
          throw (ExH::System::Exception)
            : subject_ (subject), message_ (message) {}

        MessageImpl (char const* subject, std::ostringstream const& message)
          throw (ExH::System::Exception)
            : subject_ (subject), message_ (message.str ()) {}

        virtual
        ~MessageImpl () throw () {}

        virtual std::string
        text () const throw (ExH::System::Exception)
        {
          std::string category = Category::name ();

          if (category.empty ())
          {
            return subject_ + ": " + message_;
          }
          else
          {
            return subject_ + ": " + category + ": " + message_;
          }
        }

      private:
        std::string subject_;
        std::string message_;
      };

      //
      // Error
      //

      struct ErrorCategory
      {
        virtual ~ErrorCategory () throw () {}

        std::string name () const throw (ExH::System::Exception)
        {
          return "error";
        }
      };

      typedef MessageImpl<ErrorCategory> Error;

      //
      // Warning
      //

      struct WarningCategory
      {
        virtual ~WarningCategory () throw () {}

        std::string name () const throw (ExH::System::Exception)
        {
          return "warning";
        }
      };

      typedef MessageImpl<WarningCategory> Warning;

      //
      // Information
      //

      struct InformationCategory
      {
        virtual ~InformationCategory () throw () {}

        std::string name () const throw (ExH::System::Exception)
        {
          return "";
        }
      };

      typedef MessageImpl<InformationCategory> Information;
    }
  }
}

#endif  // CCF_RUNTIME_DIAGNOSTIC_DIAGNOSTIC_STREAM_H
