// file      : CCF/IDL2/SemanticGraph/Graph.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef ELEMENT_GRAPH_HPP
#define ELEMENT_GRAPH_HPP

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

#include <iostream>

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      template <typename N, typename E>
      class Graph
      {
      public:
        ~Graph ();

      public:
        template <typename T>
        T&
        new_node ();

        template <typename T, typename A0>
        T&
        new_node (A0 const& a0);

        template <typename T, typename A0, typename A1>
        T&
        new_node (A0 const& a0, A1 const& a1);

        template <typename T, typename A0, typename A1, typename A2>
        T&
        new_node (A0 const& a0, A1 const& a1, A2 const& a2);

        template <typename T,
                  typename A0,
                  typename A1,
                  typename A2,
                  typename A3>
        T&
        new_node (A0 const& a0, A1 const& a1, A2 const& a2, A3 const& a3);

        void
        delete_node (N const& n);

      public:
        template <typename T, typename X, typename Y>
        T&
        new_edge (X& left, Y& right);

        template <typename T, typename X, typename Y,
                  typename A0>
        T&
        new_edge (X& left, Y& right, A0 const& a0);

        template <typename T, typename X, typename Y,
                  typename A0,
                  typename A1>
        T&
        new_edge (X& left, Y& right, A0 const& a0, A1 const& a1);

        void
        delete_edge (E const& e);

      protected:
        typedef N* NodePtr;
        typedef E* EdgePtr;

        typedef std::set<NodePtr> Nodes_;
        typedef std::set<EdgePtr> Edges_;

        Nodes_ nodes_;
        Edges_ edges_;
      };
    }
  }
}

#include "Graph.tpp"

#endif // ELEMENT_GRAPH_HPP
