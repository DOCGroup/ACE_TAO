// file      : CCF/IDL2/SyntaxTree/Translation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SYNTAX_TREE_TRANSLATION_HPP
#define CCF_IDL2_SYNTAX_TREE_TRANSLATION_HPP

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SyntaxTree
    {

      //
      //
      //
      class FileScope : public virtual Scope
      {
      public:
        virtual
        ~FileScope () throw () {}

        FileScope (DeclarationTable& table, Order const& order);

        virtual ScopePtr
        scope () const throw (NotInScope)
        {
          throw NotInScope ();
        }

        // Runtime declaration type information
      public:
        virtual std::string
        declaration_class ()
        {
          return "file scope";
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<FileScope>
      FileScopePtr;

      typedef
      DeclarationOrderComparator<FileScopePtr>
      FileScopeOrderComparator;

      typedef
      std::set<FileScopePtr, FileScopeOrderComparator>
      FileScopeSet;


      //
      //
      //
      class TranslationRegion;

      typedef
      StrictPtr<TranslationRegion>
      TranslationRegionPtr;

      typedef
      std::vector<TranslationRegionPtr>
      TranslationRegionList;

      class TranslationRegion : public virtual Node
      {
      public:
        virtual
        ~TranslationRegion () throw () {}

        TranslationRegion (fs::path const& file_path,
                           DeclarationTable& table,
                           Order const& order)
            : order_ (order),
              table_ (table),
              next_order_ (0),
              file_path_ (file_path)
        {
          type_info (static_type_info ());

          scope_ = FileScopePtr (new FileScope (table_, Order (order_, 1)));

          //@@ eh!
          table.insert (scope_);
        }

      public:
        DeclarationTable const&
        table () const
        {
          return table_;
        }

        DeclarationTable&
        table ()
        {
          return table_;
        }

      public:
        Order
        create_order ()
        {
          return Order (Order (order_, 0), next_order_++);
        }

      public:
        FileScopePtr
        scope ()
        {
          return scope_;
        }

      public:
        virtual fs::path
        file_path () const
        {
          return file_path_;
        }

      public:
        class InvalidArgument {};

        void
        insert (TranslationRegionPtr const& tr) throw (InvalidArgument);

        typedef
        TranslationRegionList::const_iterator
        Iterator;

        Iterator
        begin () const;

        Iterator
        end () const;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();


      private:
        Order order_;
        FileScopePtr scope_;
        DeclarationTable& table_;
        unsigned long next_order_;
        TranslationRegionList region_list_;
        fs::path file_path_;
      };


      //
      //
      //
      class IncludeTranslationRegion : public virtual TranslationRegion
      {
      public:
        virtual
        ~IncludeTranslationRegion () throw () {}

        IncludeTranslationRegion (fs::path const& file_path,
                                  DeclarationTable& table,
                                  Order const& order)
            : TranslationRegion (file_path, table, order)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<IncludeTranslationRegion>
      IncludeTranslationRegionPtr;


      //
      //
      //
      class UserIncludeTranslationRegion :
        public virtual IncludeTranslationRegion
      {
      public:
        virtual
        ~UserIncludeTranslationRegion () throw () {}

        UserIncludeTranslationRegion (fs::path const& file_path,
                                      DeclarationTable& table,
                                      Order const& order)
            : TranslationRegion (file_path, table, order),
              IncludeTranslationRegion (file_path, table, order)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<UserIncludeTranslationRegion>
      UserIncludeTranslationRegionPtr;

      //
      //
      //
      class SysIncludeTranslationRegion :
        public virtual IncludeTranslationRegion
      {
      public:
        virtual
        ~SysIncludeTranslationRegion () throw () {}

        SysIncludeTranslationRegion (fs::path const& file_path,
                                     DeclarationTable& table,
                                     Order const& order)
            : TranslationRegion (file_path, table, order),
              IncludeTranslationRegion (file_path, table, order)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<SysIncludeTranslationRegion>
      SysIncludeTranslationRegionPtr;

      //
      //
      //
      class ImpliedIncludeTranslationRegion :
        public virtual IncludeTranslationRegion
      {
      public:
        virtual
        ~ImpliedIncludeTranslationRegion () throw () {}

        ImpliedIncludeTranslationRegion (fs::path const& file_path,
                                         DeclarationTable& table,
                                         Order const& order)
            : TranslationRegion (file_path, table, order),
              IncludeTranslationRegion (file_path, table, order)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<ImpliedIncludeTranslationRegion>
      ImpliedIncludeTranslationRegionPtr;


      //
      //
      //
      class PrincipalTranslationRegion : public virtual TranslationRegion
      {
      public:
        virtual
        ~PrincipalTranslationRegion () throw () {}

        PrincipalTranslationRegion (fs::path const& file_path,
                                    DeclarationTable& table,
                                    Order const& order)
            : TranslationRegion (file_path, table, order)
        {
          type_info (static_type_info ());
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<PrincipalTranslationRegion>
      PrincipalTranslationRegionPtr;


      //
      //
      //
      class TranslationUnit : public virtual Node
      {
      public:
        virtual
        ~TranslationUnit () throw () {}

        TranslationUnit ()
            : order_ (0),
              next_order_ (0)
        {
          type_info (static_type_info ());
        }

      public:

        DeclarationTable const&
        table () const
        {
          return table_;
        }

        DeclarationTable&
        table ()
        {
          return table_;
        }

      public:
        Order
        create_order ()
        {
          return Order (order_, next_order_++);
        }

      public:
        class InvalidArgument {};

        void
        insert (TranslationRegionPtr const& tr) throw (InvalidArgument);

        typedef
        TranslationRegionList::const_iterator
        Iterator;

        Iterator
        begin () const;

        Iterator
        end () const;


        class NotFound {};

        PrincipalTranslationRegionPtr
        principal_translation_region () const throw (NotFound)
        {
          for (Iterator i = begin (); i != end (); i++)
          {
            if ((*i)->is_a<PrincipalTranslationRegion> ())
            {
              return (*i)->dynamic_type<PrincipalTranslationRegion> ();
            }
          }

          throw NotFound ();
        }

      private:
        Order order_;
        DeclarationTable table_;
        unsigned long next_order_;
        TranslationRegionList region_list_;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();
      };

      typedef
      StrictPtr<TranslationUnit>
      TranslationUnitPtr;
    }
  }
}

#endif  // CCF_IDL2_SYNTAX_TREE_TRANSLATION_HPP
