// file      : CCF/IDL2/SemanticGraph/Name.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_NAME_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_NAME_HPP

#include <vector>
#include <string>
#include <iosfwd>

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //@@ better names for names: maybe SimpleName, ScopedName, FullName?
      //
      //

      class InvalidName {};

      class SimpleName
      {
      public:
        SimpleName (char const* name) throw (InvalidName);
        SimpleName (std::string const& name) throw (InvalidName);

      public:
        bool
        operator< (SimpleName const& other) const
        {
          return name_ < other.name_;
        }

        bool
        operator== (SimpleName const& other) const
        {
          return name_ == other.name_;
        }

        bool
        operator!= (SimpleName const& other) const
        {
          return name_ != other.name_;
        }

      public:
        std::string
        str () const
        {
          return name_;
        }

      private:
        std::string name_;
      };

      SimpleName
      operator+ (SimpleName const& name, std::string const& str);

      SimpleName
      operator+ (std::string const& str, SimpleName const& name);

      std::ostream&
      operator << (std::ostream& o, SimpleName const& name);

      //
      //
      //
      class Name
      {
        typedef
        std::vector<SimpleName>
        Name_;

      public:
        Name (SimpleName const& name);

        Name (char const* name) throw (InvalidName);
        Name (std::string const& name) throw (InvalidName);

        typedef
        Name_::const_iterator
        Iterator;

        Name (Iterator begin, Iterator end) throw (InvalidName);

      public:
        bool
        operator< (Name const& other) const
        {
          return name_cache_ < other.name_cache_;
        }

        bool
        operator== (Name const& other) const
        {
          return name_cache_ == other.name_cache_;
        }

        bool
        operator!= (Name const& other) const
        {
          return name_cache_ != other.name_cache_;
        }

      public:
        Iterator
        begin () const
        {
          return name_.begin ();
        }

        Iterator
        end () const
        {
          return name_.end ();
        }

      public:
        bool
        scoped () const;

        bool
        simple () const;

      public:
        std::string
        str () const
        {
          return name_cache_;
        }

      private:
        void
        init () throw (InvalidName);

      private:
        Name_ name_;
        std::string name_cache_;
      };

      std::ostream&
      operator << (std::ostream& o, Name const& name);


      // Should always start with "::". Can be just "::" which
      // means it's a file-scope.
      //
      //
      class ScopedName : public Name
      {
      public:
        ScopedName (char const* name) throw (InvalidName);

        ScopedName (std::string const& name) throw (InvalidName);

        ScopedName (Iterator begin, Iterator end) throw (InvalidName);

        explicit
        ScopedName (Name const& name) throw (InvalidName);

        ScopedName (ScopedName const& scope, Name const& name)
          throw (InvalidName);

      public:
        SimpleName
        simple_name () const;

        class FileScope {};

        ScopedName
        scope_name () const throw (FileScope);

        /*
        Name
        in_file_scope () const throw (FileScope);
        */
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_NAME_HPP
