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
        SimpleName (char const* name);
        SimpleName (std::string const& name);

      public:
        // Comparisons are escape-insensitive.
        //
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
        bool
        escaped () const
        {
          return escaped_;
        }

        std::string
        str () const
        {
          return (escaped_ ? "_" : "") + name_;
        }

        std::string
        unescaped_str () const
        {
          return name_;
        }

      private:
        bool escaped_;
        std::string name_;
      };

      // The following operators preserve escaping.
      //
      SimpleName
      operator+ (SimpleName const& name, std::string const& str);

      SimpleName
      operator+ (std::string const& str, SimpleName const& name);


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

        Name (char const* name);
        Name (std::string const& name);

        typedef
        Name_::const_iterator
        Iterator;

        Name (Iterator begin, Iterator end);

      public:
        // Comparisons are escape-insensitive.
        //
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
          std::string r;

          for (Iterator b (begin ()), i (b), e (end ()); i != e; ++i)
            r += (i != b ? "::" : "") + i->str ();

          return r;
        }

        std::string
        unescaped_str () const
        {
          return name_cache_;
        }

      private:
        void
        init (std::string const& name);

      private:
        Name_ name_;
        std::string name_cache_; // Unescaped name cache.
      };

      // Should always start with "::". Can be just "::" which
      // means it's a file-scope.
      //
      //
      class ScopedName : public Name
      {
      public:
        ScopedName (char const* name);

        ScopedName (std::string const& name);

        ScopedName (Iterator begin, Iterator end);

        explicit
        ScopedName (Name const& name);

        ScopedName (ScopedName const& scope, Name const& name);

      public:
        SimpleName
        simple_name () const;

        class FileScope {};

        ScopedName
        scope_name () const;
      };


      // Name printer index for the stream's pword.
      //
      extern int const name_printer_index;

      //
      //
      struct NamePrinter
      {
        virtual ~NamePrinter ();

        virtual void
        print (std::ostream&, SimpleName const&) = 0;

        // The default implementation prints individul simple names
        // using the print function above seperated by '::'.
        //
        virtual void
        print (std::ostream&, Name const&);

      };
    }
  }
}

// Uses the name printer (see above) If it is installed, otherwise
// prints in the escaped form.
//
std::ostream&
operator << (std::ostream&, CCF::IDL2::SemanticGraph::SimpleName const&);

// Uses the name printer (see above) If it is installed, otherwise
// prints in the escaped form.
//
std::ostream&
operator << (std::ostream& o, CCF::IDL2::SemanticGraph::Name const& name);

#endif  // CCF_IDL2_SEMANTIC_GRAPH_NAME_HPP
