// file      : CCF/IDL2/Traversal/IntExpression.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_INT_EXPRESSION_HPP
#define CCF_IDL2_TRAVERSAL_INT_EXPRESSION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      typedef
      Node<SemanticGraph::IntExpression>
      IntExpression;

      //
      //
      typedef
      Node<SemanticGraph::IntLiteral>
      IntLiteral;

      //
      //
      typedef
      Node<SemanticGraph::IntConst>
      IntConst;


      // NEG
      //

      struct IntNegates: Edge<SemanticGraph::IntNegates>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntNeg: Node<SemanticGraph::IntNeg>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          negates (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        negates (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.negates ());
        }

        virtual void
        negates (Type& n)
        {
          negates (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // COM
      //

      struct IntComplements: Edge<SemanticGraph::IntComplements>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntCom: Node<SemanticGraph::IntCom>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          complements (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        complements (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.complements ());
        }

        virtual void
        complements (Type& n)
        {
          complements (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // MUL
      //

      struct IntMultiplies: Edge<SemanticGraph::IntMultiplies>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntMul: Node<SemanticGraph::IntMul>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          multiplies_first_factor (n);
          sign (n);
          multiplies_second_factor (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        multiplies_first_factor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.first_factor ());
        }

        virtual void
        multiplies_first_factor (Type& n)
        {
          multiplies_first_factor (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        multiplies_second_factor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. second_factor ());
        }

        virtual void
        multiplies_second_factor (Type& n)
        {
          multiplies_second_factor (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // DIV & REM
      //

      struct IntDivides: Edge<SemanticGraph::IntDivides>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntDiv: Node<SemanticGraph::IntDiv>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          divides_divident (n);
          sign (n);
          divides_divisor (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        divides_divident (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.divident ());
        }

        virtual void
        divides_divident (Type& n)
        {
          divides_divident (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        divides_divisor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. divisor ());
        }

        virtual void
        divides_divisor (Type& n)
        {
          divides_divisor (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };

      struct IntRem: Node<SemanticGraph::IntRem>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          divides_divident (n);
          sign (n);
          divides_divisor (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        divides_divident (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.divident ());
        }

        virtual void
        divides_divident (Type& n)
        {
          divides_divident (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        divides_divisor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. divisor ());
        }

        virtual void
        divides_divisor (Type& n)
        {
          divides_divisor (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // ADD
      //

      struct IntAdds: Edge<SemanticGraph::IntAdds>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntAdd: Node<SemanticGraph::IntAdd>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          adds_first_item (n);
          sign (n);
          adds_second_item (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        adds_first_item (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.first_item ());
        }

        virtual void
        adds_first_item (Type& n)
        {
          adds_first_item (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        adds_second_item (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. second_item ());
        }

        virtual void
        adds_second_item (Type& n)
        {
          adds_second_item (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // SUB
      //

      struct IntSubtracts: Edge<SemanticGraph::IntSubtracts>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntSub: Node<SemanticGraph::IntSub>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          subtracts_minuend (n);
          sign (n);
          subtracts_subtrahend (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        subtracts_minuend (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.minuend ());
        }

        virtual void
        subtracts_minuend (Type& n)
        {
          subtracts_minuend (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        subtracts_subtrahend (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. subtrahend ());
        }

        virtual void
        subtracts_subtrahend (Type& n)
        {
          subtracts_subtrahend (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // RSH & LSH
      //

      struct IntShifts: Edge<SemanticGraph::IntShifts>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntRsh: Node<SemanticGraph::IntRsh>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          shifts_pattern (n);
          sign (n);
          shifts_factor (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        shifts_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.pattern ());
        }

        virtual void
        shifts_pattern (Type& n)
        {
          shifts_pattern (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        shifts_factor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. factor ());
        }

        virtual void
        shifts_factor (Type& n)
        {
          shifts_factor (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };

      struct IntLsh: Node<SemanticGraph::IntLsh>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          shifts_pattern (n);
          sign (n);
          shifts_factor (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        shifts_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.pattern ());
        }

        virtual void
        shifts_pattern (Type& n)
        {
          shifts_pattern (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        shifts_factor (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. factor ());
        }

        virtual void
        shifts_factor (Type& n)
        {
          shifts_factor (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // AND
      //

      struct IntConjuncts: Edge<SemanticGraph::IntConjuncts>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntAnd: Node<SemanticGraph::IntAnd>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          conjuncts_first_pattern (n);
          sign (n);
          conjuncts_second_pattern (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        conjuncts_first_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.first_pattern ());
        }

        virtual void
        conjuncts_first_pattern (Type& n)
        {
          conjuncts_first_pattern (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        conjuncts_second_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. second_pattern ());
        }

        virtual void
        conjuncts_second_pattern (Type& n)
        {
          conjuncts_second_pattern (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // Xor
      //

      struct IntExclusivelyDisjuncts:
        Edge<SemanticGraph::IntExclusivelyDisjuncts>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntXor: Node<SemanticGraph::IntXor>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          exclusively_disjuncts_first_pattern (n);
          sign (n);
          exclusively_disjuncts_second_pattern (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        exclusively_disjuncts_first_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.first_pattern ());
        }

        virtual void
        exclusively_disjuncts_first_pattern (Type& n)
        {
          exclusively_disjuncts_first_pattern (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        exclusively_disjuncts_second_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. second_pattern ());
        }

        virtual void
        exclusively_disjuncts_second_pattern (Type& n)
        {
          exclusively_disjuncts_second_pattern (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };


      // Or
      //

      struct IntInclusivelyDisjuncts:
        Edge<SemanticGraph::IntInclusivelyDisjuncts>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.argument ());
        }
      };

      struct IntOr: Node<SemanticGraph::IntOr>
      {
        virtual void
        traverse (Type& n)
        {
          pre (n);
          inclusively_disjuncts_first_pattern (n);
          sign (n);
          inclusively_disjuncts_second_pattern (n);
          post (n);
        }

        virtual void
        pre (Type&)
        {
        }

        virtual void
        inclusively_disjuncts_first_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n.first_pattern ());
        }

        virtual void
        inclusively_disjuncts_first_pattern (Type& n)
        {
          inclusively_disjuncts_first_pattern (n, edge_traverser ());
        }

        virtual void
        sign (Type&)
        {
        }

        virtual void
        inclusively_disjuncts_second_pattern (Type& n, EdgeDispatcherBase& d)
        {
          d.traverse (n. second_pattern ());
        }

        virtual void
        inclusively_disjuncts_second_pattern (Type& n)
        {
          inclusively_disjuncts_second_pattern (n, edge_traverser ());
        }

        virtual void
        post (Type&)
        {
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_INT_EXPRESSION_HPP
