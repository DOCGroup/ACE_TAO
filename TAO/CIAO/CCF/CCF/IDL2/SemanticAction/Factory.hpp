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
      class Attribute;
      class Include;
      class Interface;
      class Module;
      class Operation;
      class Typedef;
      class TypeId;
      class TypePrefix;

      //
      //
      //
      class Factory
      {
      public:
        virtual
        ~Factory () throw () {}

        virtual Attribute&
        attribute () = 0;

        virtual Include&
        include () = 0;

        virtual Interface&
        interface () = 0;

        virtual Module&
        module () = 0;

        virtual Operation&
        operation () = 0;

        virtual Typedef&
        typedef_ () = 0;

        virtual TypeId&
        type_id () = 0;

        virtual TypePrefix&
        type_prefix () = 0;
      };

    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_FACTORY_HPP
