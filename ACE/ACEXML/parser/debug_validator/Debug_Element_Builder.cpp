#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/parser/debug_validator/Debug_Element_Builder.h"

ACEXML_Debug_Element_Builder::ACEXML_Debug_Element_Builder ()
  : type_ (CONTENT_TYPE::UNDEFINED),
    root_ ()
{
}

ACEXML_Debug_Element_Builder::~ACEXML_Debug_Element_Builder () = default;

int
ACEXML_Debug_Element_Builder::setElementName (ACEXML_Char const *,
                                              ACEXML_Char const *,
                                              ACEXML_Char const *qName)
{
  this->element_.set (qName, false);
  return 0;
}

int
ACEXML_Debug_Element_Builder::setContentType (CONTENT_TYPE type)
{
  if (this->type_ == CONTENT_TYPE::UNDEFINED)
    {
      this->type_ = type;
      return 0;
    }

  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("Element type redefinition in Debug_Validator.")), -1);
}

int
ACEXML_Debug_Element_Builder::insertMixedElement (ACEXML_Char const *,
                                                  ACEXML_Char const *,
                                                  ACEXML_Char const *qName)
{
  ACEXML_Element_Tree_Name_Node *raw_node = nullptr;

  // @@ We should "throw" an exception here instead of returning -1.
  ACE_NEW_RETURN (raw_node,
                  ACEXML_Element_Tree_Name_Node (qName),
                  -1);
  std::unique_ptr<ACEXML_Element_Tree_Name_Node> node (raw_node);

  if (this->root_ == nullptr)
    {
      ACEXML_Element_Tree_List_Node *raw_root = nullptr;
      ACE_NEW_RETURN (raw_root,
                      ACEXML_Element_Tree_List_Node (),
                      -1);
      this->root_.reset (raw_root);
    }

  return this->root_->insert (node.release ());
}

int
ACEXML_Debug_Element_Builder::startChildGroup ()
{
  ACEXML_Element_Tree_List_Node *raw_lnode = nullptr;

  ACE_NEW_RETURN (raw_lnode,
                  ACEXML_Element_Tree_List_Node (),
                  -1);
  std::unique_ptr<ACEXML_Element_Tree_List_Node> lnode (raw_lnode);
  ACEXML_Element_Tree_List_Node * const lnode_ptr = lnode.get ();

  if (this->root_ == nullptr)
    {
      this->root_.reset (lnode.release ());
    }
  else
    {
      // @@ check error?
      this->root_->insert (lnode.release ());
    }

  this->active_list_.push (lnode_ptr);
  return 0;
}

int
ACEXML_Debug_Element_Builder::endChildGroup (CARDINALITY )
{
  this->active_list_.pop ();
  return 0;
}

int
ACEXML_Debug_Element_Builder::setChoice ()
{
  this->active_list_.top ()->set (ACEXML_Element_Tree_List_Node::LIST_TYPE::CHOICE);
  return 0;
}

int
ACEXML_Debug_Element_Builder::setSequence ()
{
  this->active_list_.top ()->set (ACEXML_Element_Tree_List_Node::LIST_TYPE::SEQUENCE);
  return 0;
}

int
ACEXML_Debug_Element_Builder::insertElement (ACEXML_Char const *,
                                             ACEXML_Char const *,
                                             ACEXML_Char const *qName)
{
  ACEXML_Element_Tree_Name_Node *raw_node = nullptr;

  // @@ We should "throw" an exception here instead of returning -1.
  ACE_NEW_RETURN (raw_node,
                  ACEXML_Element_Tree_Name_Node (qName),
                  -1);
  std::unique_ptr<ACEXML_Element_Tree_Name_Node> node (raw_node);

  return this->active_list_.top ()->insert (node.release ());
}

void
ACEXML_Debug_Element_Builder::dump ()
{
  cout << "<!ELEMENT " << this->element_;

  // @@ Also dump element contentspec here.
  switch (this->type_)
    {
    case CONTENT_TYPE::EMPTY:
      cout << "EMPTY";
      break;
    case CONTENT_TYPE::ANY:
      cout << "ANY";
      break;
    case CONTENT_TYPE::MIXED:
    case CONTENT_TYPE::CHILDREN:
      // @@ Dump the content of this->root_
      cout << "*** not implemented ***";
      break;
    default:
      cout << "*** Unidentified element type ***";
      break;
    }

  cout << ">" << endl;
}
