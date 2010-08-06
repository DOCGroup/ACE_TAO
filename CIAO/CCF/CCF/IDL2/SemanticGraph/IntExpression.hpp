// file      : CCF/IDL2/SemanticGraph/IntExpression.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_INT_EXPRESSION_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_INT_EXPRESSION_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      class IntExpression: public virtual Expression
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntExpression () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~IntExpression () = 0;
      };


      //
      //
      class IntLiteral: public virtual IntExpression
      {
      public:
        unsigned long long
        value () const
        {
          return value_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntLiteral (Path const& path,
                    unsigned long line,
                    unsigned long long value)
            : Node (path, line), value_ (value)
        {
          type_info (static_type_info ());
        }

      private:
        unsigned long long value_;
      };


      //
      //
      class IntConst: public virtual Const,
                      public virtual IntExpression
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntConst (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      // NEG
      //

      class IntNeg;

      class IntNegates: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntNegates ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntNeg&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntNeg: public virtual IntExpression
      {
      public:
        IntNegates&
        negates () const
        {
          return *negates_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntNeg (Path const& path, unsigned long line)
            : Node (path, line), negates_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntNegates& n)
        {
          negates_ = &n;
        }

      private:
        IntNegates* negates_;
      };


      // COM
      //

      class IntCom;

      class IntComplements: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntComplements ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntCom&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntCom: public virtual IntExpression
      {
      public:
        IntComplements&
        complements () const
        {
          return *complements_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntCom (Path const& path, unsigned long line)
            : Node (path, line), complements_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntComplements& n)
        {
          complements_ = &n;
        }

      private:
        IntComplements* complements_;
      };


      // MUL
      //

      class IntMul;

      class IntMultiplies: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntMultiplies ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntMul&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntMul: public virtual IntExpression
      {
      public:
        IntMultiplies&
        first_factor () const
        {
          return *first_factor_;
        }

        IntMultiplies&
        second_factor () const
        {
          return *second_factor_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntMul (Path const& path, unsigned long line)
            : Node (path, line),
              first_factor_ (0),
              second_factor_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntMultiplies& d)
        {
          if (first_factor_ == 0)
            first_factor_ = &d;
          else
            second_factor_ = &d;
        }

      private:
        IntMultiplies* first_factor_;
        IntMultiplies* second_factor_;
      };


      // DIV & REM
      //

      class IntDiv;
      class IntRem;

      class IntDivides: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntDivides ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntDiv&)
        {
        }

        void
        set_left_node (IntRem&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntDiv: public virtual IntExpression
      {
      public:
        IntDivides&
        divident () const
        {
          return *divident_;
        }

        IntDivides&
        divisor () const
        {
          return *divisor_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntDiv (Path const& path, unsigned long line)
            : Node (path, line),
              divident_ (0),
              divisor_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntDivides& d)
        {
          if (divident_ == 0)
            divident_ = &d;
          else
            divisor_ = &d;
        }

      private:
        IntDivides* divident_;
        IntDivides* divisor_;
      };

      class IntRem: public virtual IntExpression
      {
      public:
        IntDivides&
        divident () const
        {
          return *divident_;
        }

        IntDivides&
        divisor () const
        {
          return *divisor_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntRem (Path const& path, unsigned long line)
            : Node (path, line),
              divident_ (0),
              divisor_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntDivides& d)
        {
          if (divident_ == 0)
            divident_ = &d;
          else
            divisor_ = &d;
        }

      private:
        IntDivides* divident_;
        IntDivides* divisor_;
      };


      // ADD
      //

      class IntAdd;

      class IntAdds: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntAdds ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntAdd&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntAdd: public virtual IntExpression
      {
      public:
        IntAdds&
        first_item () const
        {
          return *first_item_;
        }

        IntAdds&
        second_item () const
        {
          return *second_item_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntAdd (Path const& path, unsigned long line)
            : Node (path, line),
              first_item_ (0),
              second_item_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntAdds& d)
        {
          if (first_item_ == 0)
            first_item_ = &d;
          else
            second_item_ = &d;
        }

      private:
        IntAdds* first_item_;
        IntAdds* second_item_;
      };


      // SUB
      //

      class IntSub;

      class IntSubtracts: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntSubtracts ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntSub&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntSub: public virtual IntExpression
      {
      public:
        IntSubtracts&
        minuend () const
        {
          return *minuend_;
        }

        IntSubtracts&
        subtrahend () const
        {
          return *subtrahend_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntSub (Path const& path, unsigned long line)
            : Node (path, line),
              minuend_ (0),
              subtrahend_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntSubtracts& d)
        {
          if (minuend_ == 0)
            minuend_ = &d;
          else
            subtrahend_ = &d;
        }

      private:
        IntSubtracts* minuend_;
        IntSubtracts* subtrahend_;
      };


      // RSH & LSH
      //

      class IntRsh;
      class IntLsh;

      class IntShifts: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntShifts ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntRsh&)
        {
        }

        void
        set_left_node (IntLsh&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntRsh: public virtual IntExpression
      {
      public:
        IntShifts&
        pattern () const
        {
          return *pattern_;
        }

        IntShifts&
        factor () const
        {
          return *factor_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntRsh (Path const& path, unsigned long line)
            : Node (path, line),
              pattern_ (0),
              factor_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntShifts& d)
        {
          if (pattern_ == 0)
            pattern_ = &d;
          else
            factor_ = &d;
        }

      private:
        IntShifts* pattern_;
        IntShifts* factor_;
      };

      class IntLsh: public virtual IntExpression
      {
      public:
        IntShifts&
        pattern () const
        {
          return *pattern_;
        }

        IntShifts&
        factor () const
        {
          return *factor_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntLsh (Path const& path, unsigned long line)
            : Node (path, line),
              pattern_ (0),
              factor_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntShifts& d)
        {
          if (pattern_ == 0)
            pattern_ = &d;
          else
            factor_ = &d;
        }

      private:
        IntShifts* pattern_;
        IntShifts* factor_;
      };


      // AND
      //

      class IntAnd;

      class IntConjuncts: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntConjuncts ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntAnd&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntAnd: public virtual IntExpression
      {
      public:
        IntConjuncts&
        first_pattern () const
        {
          return *first_pattern_;
        }

        IntConjuncts&
        second_pattern () const
        {
          return *second_pattern_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntAnd (Path const& path, unsigned long line)
            : Node (path, line),
              first_pattern_ (0),
              second_pattern_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntConjuncts& d)
        {
          if (first_pattern_ == 0)
            first_pattern_ = &d;
          else
            second_pattern_ = &d;
        }

      private:
        IntConjuncts* first_pattern_;
        IntConjuncts* second_pattern_;
      };


      // XOR
      //

      class IntXor;

      class IntExclusivelyDisjuncts: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntExclusivelyDisjuncts ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntXor&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntXor: public virtual IntExpression
      {
      public:
        IntExclusivelyDisjuncts&
        first_pattern () const
        {
          return *first_pattern_;
        }

        IntExclusivelyDisjuncts&
        second_pattern () const
        {
          return *second_pattern_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntXor (Path const& path, unsigned long line)
            : Node (path, line),
              first_pattern_ (0),
              second_pattern_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntExclusivelyDisjuncts& d)
        {
          if (first_pattern_ == 0)
            first_pattern_ = &d;
          else
            second_pattern_ = &d;
        }

      private:
        IntExclusivelyDisjuncts* first_pattern_;
        IntExclusivelyDisjuncts* second_pattern_;
      };


      // OR
      //

      class IntOr;

      class IntInclusivelyDisjuncts: public virtual Edge
      {
      public:
        IntExpression&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntInclusivelyDisjuncts ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (IntOr&)
        {
        }

        void
        set_right_node (IntExpression& e)
        {
          argument_ = &e;
        }

      private:
        IntExpression* argument_;
      };


      class IntOr: public virtual IntExpression
      {
      public:
        IntInclusivelyDisjuncts&
        first_pattern () const
        {
          return *first_pattern_;
        }

        IntInclusivelyDisjuncts&
        second_pattern () const
        {
          return *second_pattern_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IntOr (Path const& path, unsigned long line)
            : Node (path, line),
              first_pattern_ (0),
              second_pattern_ (0)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (IntInclusivelyDisjuncts& d)
        {
          if (first_pattern_ == 0)
            first_pattern_ = &d;
          else
            second_pattern_ = &d;
        }

      private:
        IntInclusivelyDisjuncts* first_pattern_;
        IntInclusivelyDisjuncts* second_pattern_;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_INT_EXPRESSION_HPP
