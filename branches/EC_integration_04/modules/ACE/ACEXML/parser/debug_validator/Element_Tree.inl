// $Id$

ACEXML_INLINE
ACEXML_Element_Tree_Node::ACEXML_Element_Tree_Node ()
  : next_ (0)
{
}

ACEXML_INLINE ACEXML_Element_Tree_Node *
ACEXML_Element_Tree_Node::next ()
{
  return this->next_;
}

ACEXML_INLINE void
ACEXML_Element_Tree_Node::next (ACEXML_Element_Tree_Node * n)
{
  this->next_ = n;
}

ACEXML_INLINE
ACEXML_Element_Tree_Name_Node::ACEXML_Element_Tree_Name_Node (const ACEXML_Char *name,
                                                              int release)
  : name_ (name, 0, release)
{
}

ACEXML_INLINE void
ACEXML_Element_Tree_Name_Node::set (const ACEXML_Char *name,
                                    int release)
{
  this->name_.set (name, release);
}

ACEXML_INLINE
ACEXML_Element_Tree_List_Node::ACEXML_Element_Tree_List_Node (void)
  : type_ (SEQUENCE),
    head_ (0),
    tail_ (0),
    pop_next_ (0)
{
}

ACEXML_INLINE ACEXML_Element_Tree_List_Node::LIST_TYPE
ACEXML_Element_Tree_List_Node::get (void)
{
  return this->type_;
}

ACEXML_INLINE int
ACEXML_Element_Tree_List_Node::set (ACEXML_Element_Tree_List_Node::LIST_TYPE type)
{
  this->type_ = type;
  return 0;
}

ACEXML_INLINE
ACEXML_Element_Tree_List_Stack::ACEXML_Element_Tree_List_Stack (void)
  : top_ (0)
{
}

ACEXML_INLINE ACEXML_Element_Tree_List_Node *
ACEXML_Element_Tree_List_Stack::top ()
{
  return this->top_;
}

ACEXML_INLINE  void
ACEXML_Element_Tree_List_Stack::push (ACEXML_Element_Tree_List_Node *n)
{
  n->pop_next_ = this->top_;
  this->top_ = n;
}

ACEXML_INLINE ACEXML_Element_Tree_List_Node *
ACEXML_Element_Tree_List_Stack::pop ()
{
  if (this->top_ != 0)
    {
      ACEXML_Element_Tree_List_Node *ptr = this->top_;
      this->top_ = this->top_->pop_next_;
      return ptr;
    }
  return 0;
}

ACEXML_INLINE int
ACEXML_Element_Tree_List_Stack::empty ()
{
  return this->top_ == 0;
}
