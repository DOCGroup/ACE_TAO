// This may look like C, but it's really -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    singletons.h
//
// = DESCRIPTION
//    Type declarations for singletons used in TAO.
//
// = AUTHOR
//    Chris Cleeland, WU DOC
//
// = COPYRIGHT
//    Copyright 1997
// 
// ============================================================================

#if !defined (TAO_SINGLETONS_H)
#  define TAO_SINGLETONS_H

typedef ACE_Singleton<TAO_Marshal_Factory, ACE_SYNCH_MUTEX> TAO_MARSHAL_FACTORY;
typedef ACE_Singleton<TAO_Marshal_Primitive, ACE_SYNCH_MUTEX> TAO_MARSHAL_PRIMITIVE;
typedef ACE_Singleton<TAO_Marshal_Any, ACE_SYNCH_MUTEX> TAO_MARSHAL_ANY;
typedef ACE_Singleton<TAO_Marshal_TypeCode, ACE_SYNCH_MUTEX> TAO_MARSHAL_TYPECODE;
typedef ACE_Singleton<TAO_Marshal_Principal, ACE_SYNCH_MUTEX> TAO_MARSHAL_PRINCIPAL;
typedef ACE_Singleton<TAO_Marshal_ObjRef, ACE_SYNCH_MUTEX> TAO_MARSHAL_OBJREF;
typedef ACE_Singleton<TAO_Marshal_Struct, ACE_SYNCH_MUTEX> TAO_MARSHAL_STRUCT;
typedef ACE_Singleton<TAO_Marshal_Union, ACE_SYNCH_MUTEX> TAO_MARSHAL_UNION;
typedef ACE_Singleton<TAO_Marshal_String, ACE_SYNCH_MUTEX> TAO_MARSHAL_STRING;
typedef ACE_Singleton<TAO_Marshal_Sequence, ACE_SYNCH_MUTEX> TAO_MARSHAL_SEQUENCE;
typedef ACE_Singleton<TAO_Marshal_Array, ACE_SYNCH_MUTEX> TAO_MARSHAL_ARRAY;
typedef ACE_Singleton<TAO_Marshal_Alias, ACE_SYNCH_MUTEX> TAO_MARSHAL_ALIAS;
typedef ACE_Singleton<TAO_Marshal_Except, ACE_SYNCH_MUTEX> TAO_MARSHAL_EXCEPT;
typedef ACE_Singleton<TAO_Marshal_WString, ACE_SYNCH_MUTEX> TAO_MARSHAL_WSTRING;

// Seems like we should also provide the option for having this be
// just an ACE_Singleton<>
#if defined(TAO_HAS_TSS_ORBCORE)
#  define TAO_SINGLETON_TYPE ACE_TSS_Singleton
#else
#  define TAO_SINGLETON_TYPE ACE_Singleton
#endif

typedef TAO_SINGLETON_TYPE<TAO_ORB_Core, ACE_SYNCH_MUTEX> TAO_ORB_CORE;

#endif /* TAO_SINGLETONS_H */
