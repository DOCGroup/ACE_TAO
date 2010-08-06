// file      : CCF/IDL2/SemanticGraph/Graph.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      // Graph
      //
      //


      template <typename N, typename E>
      template <typename T>
      T& Graph<N, E>::
      new_node ()
      {
        T* n  (new T); //@@ auto
        nodes_.insert (n);
        return *n;
      }


      template <typename N, typename E>
      template <typename T, typename A0>
      T& Graph<N, E>::
      new_node (A0 const& a0)
      {
        T* n  (new T (a0)); //@@ auto
        nodes_.insert (n);
        return *n;
      }


      template <typename N, typename E>
      template <typename T, typename A0, typename A1>
      T& Graph<N, E>::
      new_node (A0 const& a0, A1 const& a1)
      {
        T* n  (new T (a0, a1)); //@@ auto
        nodes_.insert (n);
        return *n;
      }

      template <typename N, typename E>
      template <typename T, typename A0, typename A1, typename A2>
      T& Graph<N, E>::
      new_node (A0 const& a0, A1 const& a1, A2 const& a2)
      {
        T* n  (new T (a0, a1, a2)); //@@ auto
        nodes_.insert (n);
        return *n;
      }

      template <typename N, typename E>
      template <typename T,
                typename A0,
                typename A1,
                typename A2,
                typename A3>
      T& Graph<N, E>::
      new_node (A0 const& a0, A1 const& a1, A2 const& a2, A3 const& a3)
      {
        T* n  (new T (a0, a1, a2, a3)); //@@ auto
        nodes_.insert (n);
        return *n;
      }


      template <typename N, typename E>
      void Graph<N, E>::
      delete_node (N const& nr)
      {
        N* n (&nr);

        for (E* e (n->remove_edge_ ()); e != 0; e = n->remove_edge_ ())
        {
          // One endpoint of the edge has removed this edge from
          // its edge set. Now handle the other endpoint.
          //
          if (e->left_ () == n) e->right_ ()->remove_edge_ (e);
          else e->left_ ()->remove_edge_ (e);

          // Delete the edge.
          //
          edges_.erase (e);

          delete e; //@@ auto
        }

        nodes_.erase (n);

        delete n; //@@ auto
      }


      template <typename N, typename E>
      template <typename T, typename X, typename Y>
      T& Graph<N, E>::
      new_edge (X& left, Y& right)
      {
        T* e (new T); //@@ auto

        e->set_left_node (left);
        e->set_right_node (right);

        left.add_edge_left (*e);
        right.add_edge_right (*e);

        edges_.insert (e);

        return *e;
      }

      template <typename N, typename E>
      template <typename T, typename X, typename Y,
                typename A0>
      T& Graph<N, E>::
      new_edge (X& left, Y& right, A0 const& a0)
      {
        T* e (new T (a0)); //@@ auto

        e->set_left_node (left);
        e->set_right_node (right);

        left.add_edge_left (*e);
        right.add_edge_right (*e);

        edges_.insert (e);

        return *e;
      }

      template <typename N, typename E>
      template <typename T, typename X, typename Y,
                typename A0,
                typename A1>
      T& Graph<N, E>::
      new_edge (X& left, Y& right, A0 const& a0, A1 const& a1)
      {
        T* e (new T (a0, a1)); //@@ auto

        e->set_left_node (left);
        e->set_right_node (right);

        left.add_edge_left (*e);
        right.add_edge_right (*e);

        edges_.insert (e);

        return *e;
      }

      template <typename N, typename E>
      void Graph<N, E>::
      delete_edge (E const& er)
      {
        E* e (&er);

        e->left_ ()->remove_edge_ (e);
        e->right_ ()->remove_edge_ (e);

        edges_.erase (e);

        delete e; //@@ auto
      }


      template <typename N, typename E>
      Graph<N, E>::
      ~Graph ()
      {
        //@@ auto

        for (typename Edges_::iterator i (edges_.begin ()), end (edges_.end ());
             i != end;
             ++i) delete *i;

        for (typename Nodes_::iterator i (nodes_.begin ()), end (nodes_.end ());
             i != end;
             ++i) delete *i;
      }
    }
  }
}
