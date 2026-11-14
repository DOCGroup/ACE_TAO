#include "ACEXML/parser/debug_validator/Element_Tree.h"
#include "ace/streams.h"

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/debug_validator/Element_Tree.inl"
#endif /* __ACEXML_INLINE__ */

ACEXML_Element_Tree_Node::~ACEXML_Element_Tree_Node () = default;

ACE_ALLOC_HOOK_DEFINE (ACEXML_Element_Tree_Node)

void
ACEXML_Element_Tree_Name_Node::dump ()
{
  cout << this->name_;
}

ACE_ALLOC_HOOK_DEFINE (ACEXML_Element_Tree_Name_Node)

ACEXML_Element_Tree_List_Node::~ACEXML_Element_Tree_List_Node () = default;

int
ACEXML_Element_Tree_List_Node::insert (ACEXML_Element_Tree_Node *node)
{
  if (this->head_ == nullptr)
    {
      this->head_.reset (node);
      this->tail_ = node;
    }
  else
    {
      this->tail_->next (node);
      this->tail_ = node;
    }
  return 0;
}

void
ACEXML_Element_Tree_List_Node::dump ()
{
  ACEXML_Element_Tree_Node *ptr = this->head_.get ();
  ACEXML_Char const *separator =
    (this->type_ == LIST_TYPE::SEQUENCE) ? ACE_TEXT (" , ") : ACE_TEXT (" | ");

  cout << "(";

  if (ptr != nullptr)
    {
      ptr->dump ();
      ptr = ptr->next ();

      while (ptr != nullptr)
        {
          cout << separator;
          ptr->dump ();
          ptr = ptr->next ();
        }
    }

  cout << ")";
}

ACE_ALLOC_HOOK_DEFINE (ACEXML_Element_Tree_List_Node)

ACE_ALLOC_HOOK_DEFINE (ACEXML_Element_Tree_List_Stack)
