
//Author: Stoyan Paunov
//$Id$

//Node class which is used in Double_Linked_Lists


#ifndef NODE_H
#define NODE_H

#include /**/ "ace/pre.h"
#include /**/ "ace/config.h"

template <class T>
class Node {
  public:
    Node (T* t = 0);
    T* get (void);
    Node* next_;
    Node* prev_;
  private:
    T* t_;
  };

template <typename E>
class Node_Factory {
  public:
    Node_Factory () : list_ (0) {};
    ~Node_Factory () {};
  Node<E>& Create (E* element) {return new Node<E> (element);}
  private:
    Node<E>* list_;
};


#if defined (__ACE_INLINE__)
#include "Node_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Node_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#include /**/ "ace/post.h"

#endif /* NODE_H */
