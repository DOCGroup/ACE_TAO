// file      : CCF/IDL2/SemanticAction/Factory.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_ACTION_FACTORY_HPP
#define CCF_IDL2_SEMANTIC_ACTION_FACTORY_HPP

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticAction
    {
      struct Attribute;
      struct Const;
      struct Enum;
      struct Exception;
      struct Include;
      struct Interface;
      struct Member;
      struct Module;
      struct Native;
      struct NumericExpression;
      struct Operation;
      struct Struct;
      struct Typedef;
      struct TypeId;
      struct TypePrefix;
      struct Union;
      struct ValueType;
      struct ValueTypeFactory;
      struct ValueTypeMember;

      //
      //
      //
      struct Factory
      {
        virtual
        ~Factory () {}

        virtual Attribute&
        attribute () = 0;

        virtual Const&
        const_ () = 0;

        virtual SemanticAction::Enum&
        enum_ () = 0;

        virtual SemanticAction::Exception&
        exception () = 0;

        virtual Include&
        include () = 0;

        virtual Interface&
        interface () = 0;

        virtual Member&
        member () = 0;

        virtual Module&
        module () = 0;

        virtual Native&
        native () = 0;

        virtual NumericExpression&
        numeric_expression () = 0;

        virtual Operation&
        operation () = 0;

        virtual Struct&
        struct_ () = 0;

        virtual Typedef&
        typedef_ () = 0;

        virtual TypeId&
        type_id () = 0;

        virtual TypePrefix&
        type_prefix () = 0;

        virtual Union&
        union_ () = 0;

        virtual ValueType&
        value_type () = 0;

        virtual ValueTypeFactory&
        value_type_factory () = 0;

        virtual ValueTypeMember&
        value_type_member () = 0;
      };

    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_FACTORY_HPP
