// $Id$
// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//    any_test_i.cpp
//
// = AUTHOR
//    Michael Kircher
//
// = DESCRIPTION
//   Implementation of the Nodes for the any evaluator.
//
// ============================================================================

#include "Node.h"

ACE_RCSID(DOVEMIB, Node, "$Id$")

StructNode::StructNode (const char *Name_ptr,
                        unsigned int recursion_level) {
  Name_ptr_ = Name_ptr;
  queue_ptr_ = new ACE_Unbounded_Queue<Node *>;
  recursion_level_ = recursion_level;
  queue_iterator_ptr_ = 0;
}


StructNode::~StructNode () {
  for (unsigned int i = 0; i < getChildNumber (); i++) {
    delete getChild (i);
  }
  delete queue_ptr_;
  delete queue_iterator_ptr_;
}


unsigned int
StructNode::getChildNumber () {
  return (unsigned int) queue_ptr_->size ();
}

Node *
StructNode::getChild (unsigned int n) {

  // create new iterator if some things have changed
  if (queue_iterator_ptr_ == 0) {
    queue_iterator_ptr_ = new ACE_Unbounded_Queue_Iterator<Node *> (*queue_ptr_);
    queue_position_ = 0;
  }

  // check if we have such a Zebra
  if (n < queue_ptr_->size ()) {
    // if we are already to far in the queue
    if (queue_position_ >  n) {
      queue_position_ = 0;
      queue_iterator_ptr_->first ();
    }

    Node **node_ptr_ptr_;
    for (;
              !queue_iterator_ptr_->done() && queue_position_ <= n;
              queue_iterator_ptr_->advance (), queue_position_++) {
      queue_iterator_ptr_->next (node_ptr_ptr_);
    }
    return (*node_ptr_ptr_);
  }
  else {
    return 0;
  }
}

int
StructNode::addChild (Node *node) {
  delete queue_iterator_ptr_;
  queue_iterator_ptr_ = 0;
  return queue_ptr_->enqueue_tail (node);
}

const char *
StructNode::getName () {
  return Name_ptr_;
}

void
StructNode::Accept (NodeVisitor *nodeVisitor) {
  nodeVisitor->visitStructNode (this);
}

unsigned int
StructNode::getRecursionLevel () {
  return recursion_level_;
}

DoubleNode::DoubleNode (CORBA::Double *Double_ptr,
                        const char *Name_ptr,
                        unsigned int recursion_level)  {
  Double_ptr_ = Double_ptr;
  Name_ptr_ = Name_ptr;
  recursion_level_ = recursion_level;
}

const char *
DoubleNode::getName () {
  return Name_ptr_;
}



CORBA::Double
DoubleNode::getValue () {
  return *Double_ptr_;
}

void
DoubleNode::Accept (NodeVisitor *nodeVisitor) {
  nodeVisitor->visitDoubleNode (this);
}

unsigned int
DoubleNode::getRecursionLevel () {
  return recursion_level_;
}

LongNode::LongNode (CORBA::Long *Long_ptr,
                    const char *Name_ptr,
                    unsigned int recursion_level) {
  Long_ptr_ = Long_ptr;
  Name_ptr_ = Name_ptr;
  recursion_level_ = recursion_level;
}

const char *
LongNode::getName () {
  return Name_ptr_;
}

CORBA::Long
LongNode::getValue () {
  return *Long_ptr_;
}

void
LongNode::Accept (NodeVisitor *nodeVisitor) {
  nodeVisitor->visitLongNode (this);
}

unsigned int
LongNode::getRecursionLevel () {
  return recursion_level_;
}

ULongNode::ULongNode (CORBA::ULong *ULong_ptr,
                      const char *Name_ptr,
                      unsigned int recursion_level) {
  ULong_ptr_ = ULong_ptr;
  Name_ptr_ = Name_ptr;
  recursion_level_ = recursion_level;
}

const char *
ULongNode::getName () {
  return Name_ptr_;
}

CORBA::ULong
ULongNode::getValue () {
  return *ULong_ptr_;
}

void
ULongNode::Accept (NodeVisitor *nodeVisitor) {
  nodeVisitor->visitULongNode (this);
}

unsigned int
ULongNode::getRecursionLevel () {
  return recursion_level_;
}


StringNode::StringNode (CORBA::String_var String_var,
                        const char *Name_ptr,
                        unsigned int recursion_level) {
  String_var_ = String_var;
  Name_ptr_ = Name_ptr;
  recursion_level_ = recursion_level;
}

const char*
StringNode::getName () {
  return Name_ptr_;
}

CORBA::String_var
StringNode::getValue () {
  return String_var_;
}

void
StringNode::Accept (NodeVisitor *nodeVisitor) {
  nodeVisitor->visitStringNode (this);
}

unsigned int
StringNode::getRecursionLevel () {
  return recursion_level_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<Node *>;
template class ACE_Unbounded_Queue<Node *>;
template class ACE_Unbounded_Queue_Iterator<Node *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<Node *>
#pragma instantiate ACE_Unbounded_Queue<Node *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Node *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
