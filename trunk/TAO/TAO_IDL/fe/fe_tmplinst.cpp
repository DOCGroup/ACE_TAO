// $Id$

#include "ast_component.h"
#include "ace/ACE.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Containers_T.h"
#include "ace/Null_Mutex.h"

class AST_Decl;
class AST_Field;
class AST_Interface;
class AST_ValueType;
class Identifier;

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, 
                                  int>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, 
                                             int, 
                                             ACE_Hash<ACE_CString>, 
                                             ACE_Equal_To<ACE_CString>, 
                                             ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, 
                                        int, 
                                        ACE_Hash<ACE_CString>, 
                                        ACE_Equal_To<ACE_CString>, 
                                        ACE_Null_Mutex>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, 
                                                int, 
                                                ACE_Hash<ACE_CString>, 
                                                ACE_Equal_To<ACE_CString>, 
                                                ACE_Null_Mutex>;
template class ACE_Hash<ACE_CString>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, 
                                       int, 
                                       ACE_Hash<ACE_CString>, 
                                       ACE_Equal_To<ACE_CString>, 
                                       ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, 
                                    int, 
                                    ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ACE_CString, 
                                    ACE_CString, 
                                    ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, 
                                  ACE_CString>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, 
                                             ACE_CString, 
                                             ACE_Hash<ACE_CString>, 
                                             ACE_Equal_To<ACE_CString>, 
                                             ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, 
                                        ACE_CString, 
                                        ACE_Hash<ACE_CString>, 
                                        ACE_Equal_To<ACE_CString>, 
                                        ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, 
                                                ACE_CString, 
                                                ACE_Hash<ACE_CString>, 
                                                ACE_Equal_To<ACE_CString>, 
                                                ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, 
                                       ACE_CString, 
                                       ACE_Hash<ACE_CString>, 
                                       ACE_Equal_To<ACE_CString>, 
                                       ACE_Null_Mutex>;

template class ACE_Node<AST_Decl *>;
template class ACE_Unbounded_Set<AST_Decl *>;
template class ACE_Unbounded_Set_Iterator<AST_Decl *>;

template class ACE_Node<AST_Field *>;
template class ACE_Unbounded_Queue<AST_Field *>;
template class ACE_Unbounded_Queue_Iterator<AST_Field *>;

template class ACE_Node<AST_Interface *>;
template class ACE_Unbounded_Queue<AST_Interface *>;
template class ACE_Unbounded_Queue_Iterator<AST_Interface *>;

template class ACE_Node<AST_Component::port_description>;
template class ACE_Unbounded_Queue<AST_Component::port_description>;
template class ACE_Unbounded_Queue_Iterator<AST_Component::port_description>;

template class ACE_Node<Identifier *>;
template class ACE_Unbounded_Queue<Identifier *>;
template class ACE_Unbounded_Queue_Iterator<Identifier *>;

template class ACE_Node<AST_Operation *>;
template class ACE_Unbounded_Queue<AST_Operation *>;
template class ACE_Unbounded_Queue_Iterator<AST_Operation *>;

template class ACE_Unbounded_Stack<char *>;
template class ACE_Unbounded_Stack_Iterator<char *>;

template class ACE_Node<char *>;
template class ACE_Unbounded_Set<char *>;
template class ACE_Unbounded_Set_Iterator<char *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, 
                                       int>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, 
                                                  int, 
                                                  ACE_Hash<ACE_CString>, 
                                                  ACE_Equal_To<ACE_CString>, 
                                                  ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, 
                                             int, 
                                             ACE_Hash<ACE_CString>, 
                                             ACE_Equal_To<ACE_CString>, 
                                             ACE_Null_Mutex>
#pragma instantiate ACE_Equal_To<ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, 
                                                     int, 
                                                     ACE_Hash<ACE_CString>, 
                                                     ACE_Equal_To<ACE_CString>, 
                                                     ACE_Null_Mutex>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, 
                                            int, 
                                            ACE_Hash<ACE_CString>, 
                                            ACE_Equal_To<ACE_CString>, 
                                            ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, 
                                         int, 
                                         ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, 
                                       ACE_CString>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, 
                                                  ACE_CString, 
                                                  ACE_Hash<ACE_CString>, 
                                                  ACE_Equal_To<ACE_CString>, 
                                                  ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, 
                                             ACE_CString, 
                                             ACE_Hash<ACE_CString>, 
                                             ACE_Equal_To<ACE_CString>, 
                                             ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, 
                                                     ACE_CString, 
                                                     ACE_Hash<ACE_CString>, 
                                                     ACE_Equal_To<ACE_CString>, 
                                                     ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, 
                                            ACE_CString, 
                                            ACE_Hash<ACE_CString>, 
                                            ACE_Equal_To<ACE_CString>, 
                                            ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, 
                                         ACE_CString, 
                                         ACE_Null_Mutex>

#pragma instantiate ACE_Node<AST_Decl *>
#pragma instantiate ACE_Unbounded_Set<AST_Decl *>
#pragma instantiate ACE_Unbounded_Set_Iterator<AST_Decl *>

#pragma instantiate ACE_Node<AST_Field *>
#pragma instantiate ACE_Unbounded_Queue<AST_Field *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<AST_Field *>

#pragma instantiate ACE_Node<AST_Component::port_description>
#pragma instantiate ACE_Unbounded_Queue<AST_Component::port_description>
#pragma instantiate ACE_Unbounded_Queue_Iterator<AST_Component::port_description>

#pragma instantiate ACE_Node<Identifier *>
#pragma instantiate ACE_Unbounded_Queue<Identifier *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<Identifier *>

#pragma instantiate ACE_Node<AST_Operation *>
#pragma instantiate ACE_Unbounded_Queue<AST_Operation *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<AST_Operation *>

#pragma instantiate ACE_Unbounded_Stack<char *>
#pragma instantiate ACE_Unbounded_Stack_Iterator<char *>

#pragma instantiate ACE_Node<char *>
#pragma instantiate ACE_Unbounded_Set<char *>
#pragma instantiate ACE_Unbounded_Set_Iterator<char *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
