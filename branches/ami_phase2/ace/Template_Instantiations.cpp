// $Id$

// Note: this file has been created through concatenation of ALL
// explicit template instantiations in ACE, and only those that
// interfered with the multithreaded port of ACE to pSOS using the
// Diab D-C++ 4.1a compiler have been wrapped with the appropriate
// preprocesor directives for conditional compilation.  If you are
// trying to use this file, you will need to ensure that any
// additional preprocessor directives needed are added.  Also, if you
// wish to extend this technique to instantiation pragmas, you will
// need to modify the preprocesor directives below

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_FILE)

// header files that are not already included
#include "ace/Local_Tokens.h"
#include "ace/Token.h"
#include "ace/Containers.h"
#include "ace/Obstack.h"
#include "ace/Select_Reactor.h"
#include "ace/Message_Queue.h"
#include "ace/Module.h"
#include "ace/Name_Space.h"
#include "ace/Stream.h"
#include "ace/Stream_Modules.h"
#include "ace/Dynamic.h"
#include "ace/Local_Name_Space_T.h"
#include "ace/Filecache.h"
#include "ace/Token_Invariants.h"
#include "ace/Remote_Tokens.h"
#include "ace/Singleton.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_Wheel_T.h"
#include "ace/Auto_Ptr.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#error ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_FILE may not be used with  ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION or ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA
#endif /* defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION) || defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA) */

//  D E P T H = 0
// Templates with nesting depth 0 (no dependencies on
// other templates), ordered by number of template arguments

ACE_MT (template class ACE_Guard<ACE_Recursive_Thread_Mutex>);

// ACE_MT (template class ACE_Guard<ACE_SELECT_REACTOR_MUTEX>);
ACE_MT (template class ACE_Guard<ACE_Local_Mutex>);
ACE_MT (template class ACE_Guard<ACE_Token>);

ACE_MT (template class ACE_Guard<ACE_Thread_Mutex>);

template class ACE_Guard<ACE_Lock>;
template class ACE_Guard<ACE_Null_Mutex>;
template class ACE_Guard<ACE_Process_Mutex>;
template class ACE_Guard<ACE_RW_Process_Mutex>;
template class ACE_Guard<ACE_RW_Thread_Mutex>;

ACE_MT (template class ACE_TSS_Guard<ACE_Recursive_Thread_Mutex>);

template class ACE_Array<ACE_Mutex *>;
template class ACE_Array_Base<ACE_Mutex *>;
template class ACE_Array<ACE_RW_Thread_Mutex *>;
template class ACE_Array_Base<ACE_RW_Thread_Mutex *>;
template class ACE_Array<ACE_TSS_Info>;
template class ACE_Array_Base<ACE_TSS_Info>;
template class ACE_Array<ACE_Thread_Mutex *>;
template class ACE_Array_Base<ACE_Thread_Mutex *>;
template class ACE_Array_Iterator<ACE_TSS_Info>;

// ACE_PROCESS_MUTEX is defined in Malloc.h,
// depending on whether ACE_HAS_THREADS is defined
#if defined (ACE_HAS_MALLOC_STATS)
template class ACE_Atomic_Op<ACE_PROCESS_MUTEX, int>;
#endif /* ACE_HAS_MALLOC_STATS */

template class ACE_Auto_Array_Ptr<struct ifreq>;
template class ACE_Auto_Basic_Array_Ptr<struct ifreq>;
template class ACE_Auto_Basic_Ptr<ACE_Obstack>;

template class ACE_Cleanup_Adapter<ACE_Log_Msg>;
template class ACE_Cleanup_Adapter<ACE_Mutex>;
template class ACE_Cleanup_Adapter<ACE_Null_Mutex>;
template class ACE_Cleanup_Adapter<ACE_Recursive_Thread_Mutex>;

// ACE_SYNCH_RW_MUTEX is defined in Synch_T.h
template class ACE_Cleanup_Adapter<ACE_SYNCH_RW_MUTEX>;

template class ACE_Cleanup_Adapter<ACE_Thread_Mutex>;
template class ACE_Double_Linked_List<ACE_Thread_Descriptor>;
template class ACE_Double_Linked_List_Iterator<ACE_Thread_Descriptor>;

// ACE_SYNCH_RECURSIVE_MUTEX is defined in Synch_T.h
template class ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>;

template class ACE_Lock_Adapter<ACE_Select_Reactor_Token>;

template class ACE_Managed_Object<ACE_Mutex>;
template class ACE_Managed_Object<ACE_Null_Mutex>;
template class ACE_Managed_Object<ACE_Recursive_Thread_Mutex>;

// ACE_SYNCH_RW_MUTEX is defined in Synch_T.h
template class ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>;
template class ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>;

template class ACE_Managed_Object<ACE_Thread_Mutex>;

// ACE_NULL_SYNCH is defined in Synch_T.h
template class ACE_Message_Queue<ACE_NULL_SYNCH>;

// ACE_SYNCH is defined in Synch_T.h
template class ACE_Message_Queue<ACE_SYNCH>;

// ACE_NULL_SYNCH is defined in Synch_T.h
template class ACE_Module<ACE_NULL_SYNCH>;

// ACE_SYNCH is defined in Synch_T.h
template class ACE_Module<ACE_SYNCH>;

template class ACE_Node <ACE_TPQ_Entry *>;
template class ACE_Node<ACE_Cleanup_Info>;
template class ACE_Node<ACE_Event_Tuple>;
template class ACE_Node<ACE_Name_Binding>;
template class ACE_Node<ACE_Static_Svc_Descriptor *>;
template class ACE_Node<ACE_TSS_Ref>;
template class ACE_Node<ACE_Thread_Descriptor*>;
template class ACE_Node<ACE_Thread_Descriptor>;

template class ACE_Node<ACE_WString>;

// ASYS_TCHAR is defined in OS.h
template class ACE_Node<ASYS_TCHAR *>;

template class ACE_Read_Guard<ACE_Null_Mutex>;
template class ACE_Read_Guard<ACE_RW_Process_Mutex>;
template class ACE_Read_Guard<ACE_RW_Thread_Mutex>;

// ACE_SYNCH is defined in Synch_T.h
template class ACE_Stream<ACE_SYNCH>;
template class ACE_Stream_Head<ACE_SYNCH>;
template class ACE_Stream_Tail<ACE_SYNCH>;

template class ACE_TSS <ACE_SOCK_Stream>;
template class ACE_TSS <ACE_TPQ_Entry>;
template class ACE_TSS<ACE_Dynamic>;
template class ACE_TSS<ACE_TSS_Keys>;
template class ACE_TSS<ACE_Thread_Exit>;

// ACE_NULL_SYNCH is defined in Synch_T.h
template class ACE_Task<ACE_NULL_SYNCH>;

// ACE_SYNCH is defined in Synch_T.h
template class ACE_Task<ACE_SYNCH>;

// ACE_NULL_SYNCH is defined in Synch_T.h
template class ACE_Thru_Task<ACE_NULL_SYNCH>;

// ACE_SYNCH is defined in Synch_T.h
template class ACE_Thru_Task<ACE_SYNCH>;

template class ACE_Timer_Node_T<ACE_Event_Handler *>;
template class ACE_Unbounded_Queue<ACE_Cleanup_Info>;
template class ACE_Unbounded_Queue<ACE_Event_Tuple>;
template class ACE_Unbounded_Queue<ACE_Thread_Descriptor*>;
template class ACE_Unbounded_Queue<ACE_Thread_Descriptor>;

// ASYS_TCHAR is defined in OS.h
template class ACE_Unbounded_Queue<ASYS_TCHAR *>;

template class ACE_Unbounded_Queue_Iterator<ACE_Cleanup_Info>;
template class ACE_Unbounded_Queue_Iterator<ACE_Event_Tuple>;
template class ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor*>;
template class ACE_Unbounded_Queue_Iterator<ACE_Thread_Descriptor>;

// ASYS_TCHAR is defined in OS.h
template class ACE_Unbounded_Queue_Iterator<ASYS_TCHAR *>;

template class ACE_Unbounded_Set<ACE_Name_Binding>;
template class ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set<ACE_WString>;
template class ACE_Unbounded_Set_Iterator<ACE_Name_Binding>;
template class ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set_Iterator<ACE_WString>;
template class ACE_Unbounded_Stack <ACE_TPQ_Entry *>;
template class ACE_Write_Guard<ACE_Null_Mutex>;
template class ACE_Write_Guard<ACE_RW_Process_Mutex>;
template class ACE_Write_Guard<ACE_RW_Thread_Mutex>;

// ACE_SYNCH_RW_MUTEX is defined in Synch_T.h
template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;

template class auto_ptr<ACE_Obstack>;

// from Signal.cpp
#if !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#define ACE_MAX_SIGNAL_HANDLERS ((size_t) 20)
template class ACE_Fixed_Set<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>;
template class ACE_Fixed_Set_Iterator<ACE_Event_Handler *, ACE_MAX_SIGNAL_HANDLERS>;
#endif /* !defined (ACE_HAS_BROKEN_HPUX_TEMPLATES) */

// ACE_LOCAL_MEMORY_POOL, ACE_MMAP_MEMORY_POOL, and
// ACE_LITE_MMAP_MEMORY_POOL are all defined in OS.h

template class ACE_Local_Name_Space <ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Local_Name_Space <ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Malloc <ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc<ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex> >;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> >;

template class ACE_Map_Entry <ACE_Token_Name, ACE_Tokens *>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_Mutex_Invariants *>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_RWLock_Invariants *>;
template class ACE_Map_Entry<ACE_Token_Name, ACE_Token_Proxy *>;
template class ACE_Module<ACE_Thread_Mutex, ACE_Condition_Thread_Mutex>;

// from Remote_Tokens.cpp
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
template class ACE_Singleton <ACE_TSS_Connection, ACE_Thread_Mutex>;
#else
template class ACE_Singleton <ACE_TSS_Connection, ACE_Null_Mutex>;
#endif /* ACE_MT_SAFE */

template class ACE_TSS_Singleton<ACE_Dynamic, ACE_Null_Mutex>;

// from Local_Name_Space.cpp
#if (1)
template class ACE_Hash_Map_Entry<ACE_NS_String, ACE_NS_Internal>;
template class ACE_Hash<ACE_NS_String>;
template class ACE_Equal_To<ACE_NS_String>;
template class ACE_Hash_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_NS_String, ACE_NS_Internal, ACE_Hash<ACE_NS_String>, ACE_Equal_To<ACE_NS_String>, ACE_Null_Mutex>;
#else
template class ACE_Map_Entry<ACE_NS_String, ACE_NS_Internal>;
template class ACE_Map_Manager<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_NS_String, ACE_NS_Internal, ACE_Null_Mutex>;
#endif

// from Filecache.cpp
#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
template class ACE_Hash_Map_Entry<const char *, ACE_Filecache_Object *>;
template class ACE_Hash_Map_Manager<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<const char *, ACE_Filecache_Object *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, ACE_Filecache_Object *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *, ACE_Filecache_Object *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, ACE_Filecache_Object *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
#else
template class ACE_Hash_Map_Entry<ACE_CString, ACE_Filecache_Object *>;
template class ACE_Hash<ACE_CString>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Manager<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_Filecache_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_Filecache_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_Filecache_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_Filecache_Object *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

template class ACE_Map_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Token_Name, ACE_Tokens *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Token_Name, ACE_Tokens *, ACE_Null_Mutex>;
template class ACE_Map_Manager <ACE_Token_Name, ACE_Tokens *, ACE_Null_Mutex>;
template class ACE_Map_Manager<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Manager<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Manager<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_Mutex_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_RWLock_Invariants *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_Token_Proxy *, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Token_Name, ACE_Tokens *, ACE_Null_Mutex>;

//  D E P T H = 1
// Templates with nesting depth 1 (dependencies on other templates 1
// level deep), ordered by number of template arguments at top level


// ACE_MMAP_MEMORY_POOL is defined in OS.h

template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> >;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> >;

template class ACE_Node<ACE_Timer_Node_T<ACE_Event_Handler *> *>;

// ACE_SYNCH_NULL_MUTEX is defined in Synch_T.h
template class ACE_Cached_Allocator<ACE_Node<ACE_Event_Tuple>, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >;
template class ACE_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *> >;

// ACE_SYNCH_RECURSIVE_MUTEX is defined in Synch_T.h
template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Heap_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_List_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Queue_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Wheel_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;

template class ACE_Unbounded_Set<ACE_Timer_Node_T<ACE_Event_Handler *> *>;
template class ACE_Unbounded_Set_Iterator<ACE_Timer_Node_T<ACE_Event_Handler *> *>;
template class ACE_Locked_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *>, ACE_Null_Mutex>;

template class ACE_Timer_Hash_Upcall<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>;

// ACE_SYNCH_RECURSIVE_MUTEX is defined in Synch_T.h
template class ACE_Timer_Hash_Upcall <ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Timer_Hash_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX, ACE_Hash_Timer_List>;
template class ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX, ACE_Hash_Timer_List>;
template class ACE_Timer_Hash_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX, ACE_Hash_Timer_Heap>;
template class ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, ACE_SYNCH_RECURSIVE_MUTEX, ACE_Hash_Timer_Heap>;

//  D E P T H = 2
// Templates with nesting depth 2 (dependencies on other templates 2
// levels deep), ordered by number of template arguments at top level

template class ACE_Timer_Queue_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

template class ACE_Timer_Queue_Iterator_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

template class ACE_Timer_List_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

template class ACE_Timer_List_Iterator_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

template class ACE_Timer_Heap_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

template class ACE_Timer_Heap_Iterator_T <ACE_Event_Handler *, ACE_Hash_Upcall, ACE_Null_Mutex>;

// ACE_MMAP_MEMORY_POOL and ACE_LITE_MMAP_MEMORY_POOL are defined in OS.h

template class ACE_Name_Space_Map <ACE_Allocator_Adapter <ACE_Malloc <ACE_LITE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> > >;
template class ACE_Name_Space_Map <ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_RW_Process_Mutex> > >;

template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> > >;

// ACE_SYNCH_NULL_MUTEX is defined in Synch_T.h
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_Node<ACE_Event_Tuple> >, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Timer_Heap_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;

template class ACE_Timer_Heap_Iterator_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;

template class ACE_Timer_List_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;

template class ACE_Timer_List_Iterator_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;

template class ACE_Timer_Queue_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;

template class ACE_Timer_Queue_Iterator_T<ACE_Event_Handler*, ACE_Timer_Hash_Upcall<ACE_Event_Handler*, ACE_Event_Handler_Handle_Timeout_Upcall<ACE_Null_Mutex>, ACE_Null_Mutex>, ACE_Null_Mutex>;


#endif /* defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_FILE) */
