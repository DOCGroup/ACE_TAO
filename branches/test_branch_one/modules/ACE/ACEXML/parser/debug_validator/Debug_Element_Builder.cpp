// $Id$

#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/parser/debug_validator/Debug_Element_Builder.h"

ACEXML_Debug_Element_Builder::ACEXML_Debug_Element_Builder ()
  : type_ (UNDEFINED),
    root_ (0)
{
}

ACEXML_Debug_Element_Builder::~ACEXML_Debug_Element_Builder ()
{
  delete this->root_;
}

int
ACEXML_Debug_Element_Builder::setElementName (const ACEXML_Char *,
                                              const ACEXML_Char *,
                                              const ACEXML_Char *qName ACEXML_ENV_ARG_DECL_NOT_USED)
{
  this->element_.set (qName, 0);
  return 0;
}

int
ACEXML_Debug_Element_Builder::setContentType (CONTENT_TYPE type ACEXML_ENV_ARG_DECL)
{
  if (this->type_ == UNDEFINED)
    {
      this->type_ = type;
      return 0;
    }

  ACEXML_THROW_RETURN (ACEXML_SAXParseException (ACE_TEXT("Element type redefinition in Debug_Validator.")), -1);
}

int
ACEXML_Debug_Element_Builder::insertMixedElement (const ACEXML_Char *,
                                                  const ACEXML_Char *,
                                                  const ACEXML_Char *qName ACEXML_ENV_ARG_DECL_NOT_USED)
{
  ACEXML_Element_Tree_Name_Node *node;

  // @@ We should "throw" an exception here instead of returning -1.
  ACE_NEW_RETURN (node,
                  ACEXML_Element_Tree_Name_Node (qName),
                  -1);

  if (this->root_ == 0)
    // @@ Memory leak if fail?
    ACE_NEW_RETURN (this->root_,
                    ACEXML_Element_Tree_List_Node (),
                    -1);


  return this->root_->insert (node);
}

int
ACEXML_Debug_Element_Builder::startChildGroup ()
{
  ACEXML_Element_Tree_List_Node *lnode;

  ACE_NEW_RETURN (lnode,
                  ACEXML_Element_Tree_List_Node (),
                  -1);

  if (this->root_ == 0)
    {
      this->root_ = lnode;
    }
  else
    {
      // @@ check error?
      this->root_->insert (lnode);
    }

  this->active_list_.push (lnode);
  return 0;
}

int
ACEXML_Debug_Element_Builder::endChildGroup (CARDINALITY  ACEXML_ENV_ARG_DECL_NOT_USED)
{
  this->active_list_.pop ();
  return 0;
}

int
ACEXML_Debug_Element_Builder::setChoice ()
{
  this->active_list_.top ()->set (ACEXML_Element_Tree_List_Node::CHOICE);
  return 0;
}

int
ACEXML_Debug_Element_Builder::setSequence ()
{
  this->active_list_.top ()->set (ACEXML_Element_Tree_List_Node::SEQUENCE);
  return 0;
}

int
ACEXML_Debug_Element_Builder::insertElement  (const ACEXML_Char *,
                                              const ACEXML_Char *,
                                              const ACEXML_Char *qName ACEXML_ENV_ARG_DECL_NOT_USED)
{
  ACEXML_Element_Tree_Name_Node *node;

  // @@ We should "throw" an exception here instead of returning -1.
  ACE_NEW_RETURN (node,
                  ACEXML_Element_Tree_Name_Node (qName),
                  -1);

  return this->active_list_.top ()->insert (node);
}

void
ACEXML_Debug_Element_Builder::dump ()
{
  cout << "<!ELEMENT " << this->element_;

  // @@ Also dump element contentspec here.
  switch (this->type_)
    {
    case EMPTY:
      cout << "EMPTY";
      break;
    case ANY:
      cout << "ANY";
      break;
    case MIXED:
    case CHILDREN:
      // @@ Dump the content of this->root_
      cout << "*** not implemented ***";
      break;
    default:
      cout << "*** Unidentified element type ***";
      break;
    }

  cout << ">" << endl;
}
