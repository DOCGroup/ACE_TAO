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
      class Include;
      class Module;
      class Interface;
      class Attribute;
      class Operation;

      //
      //
      //
      class Factory
      {
      public:
        virtual
        ~Factory () throw () {}

        virtual Include&
        include () = 0;

        virtual Module&
        module () = 0;

        virtual Interface&
        interface () = 0;

        virtual Attribute&
        attribute () = 0;

        virtual Operation&
        operation () = 0;
      };

    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_ACTION_FACTORY_HPP
