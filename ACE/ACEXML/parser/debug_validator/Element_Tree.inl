ACEXML_INLINE
ACEXML_Element_Tree_Node::ACEXML_Element_Tree_Node ()
  : next_ (nullptr)
{
}

ACEXML_INLINE ACEXML_Element_Tree_Node *
ACEXML_Element_Tree_Node::next () const
{
  return this->next_;
}

ACEXML_INLINE void
ACEXML_Element_Tree_Node::next (ACEXML_Element_Tree_Node * n)
{
  this->next_ = n;
}

ACEXML_INLINE
ACEXML_Element_Tree_Name_Node::ACEXML_Element_Tree_Name_Node (ACEXML_Char const *name,
                                                              bool release)
  : name_ (name, nullptr, release)
{
}

ACEXML_INLINE void
ACEXML_Element_Tree_Name_Node::set (ACEXML_Char const *name,
                                    bool release)
{
  this->name_.set (name, release);
}

ACEXML_INLINE
ACEXML_Element_Tree_List_Node::ACEXML_Element_Tree_List_Node ()
  : type_ (LIST_TYPE::SEQUENCE),
    head_ (nullptr),
    tail_ (nullptr),
    pop_next_ (nullptr)
{
}

ACEXML_INLINE ACEXML_Element_Tree_List_Node::LIST_TYPE
ACEXML_Element_Tree_List_Node::get () const
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
ACEXML_Element_Tree_List_Stack::ACEXML_Element_Tree_List_Stack ()
  : top_ (nullptr)
{
}

ACEXML_INLINE ACEXML_Element_Tree_List_Node *
ACEXML_Element_Tree_List_Stack::top () const
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
  if (this->top_ != nullptr)
    {
      ACEXML_Element_Tree_List_Node *ptr = this->top_;
      this->top_ = this->top_->pop_next_;
      return ptr;
    }
  return nullptr;
}

ACEXML_INLINE bool
ACEXML_Element_Tree_List_Stack::empty () const
{
  return this->top_ == nullptr;
}
