#ifndef __STL_H
#define __STL_H
#endif

#include <bool.h>

// Assumes MFC is included too.

//*****************************************************************************
//
// SequenceDelete
//
// DESCRIPTION:
//
//      Deletes items in an STL sequence container.  All items in the 
//      container must be pointers to objects.
//
// INPUT PARAMETERS:
//      first - An iterator.  Should point to the first item to delete.
//      last - An iterator.  Should point AFTER the last item you wish
//             to delete.
//
//*****************************************************************************

template <class ForwardIteratorClass>
void 
SequenceDelete(ForwardIteratorClass first, const ForwardIteratorClass &last)
{ 
  using namespace std;

  while ( first != last )
  {
    delete *first++;
  }
}

//*****************************************************************************
//
// MapDelete
//
// DESCRIPTION:
//
//      Deletes items in an STL map container.  All items in the container
//      must be pointers to objects.
//
// INPUT PARAMETERS:
//      first - An iterator.  Should point to the first item to delete.
//      last - An iterator.  Should point AFTER the last item you wish
//             to delete.
//
//*****************************************************************************

template <class ForwardIteratorClass>
void 
MapDelete(ForwardIteratorClass first, const ForwardIteratorClass &last)
{ 
  using namespace std;

  while ( first != last )
  {
    delete (*first++).second;
  }
}

//*****************************************************************************
//
// Comparison operators
//
// STL cannot find comparison operators at the global level.  Is this a
// compier bug??
//
// These functions cannot be templates because they will cause ambiguity 
// errors within the STL code.
//
//*****************************************************************************

//*****************************************************************************
// STL_ROUTE_ALL_GLOBALS(T1, T2) (macro)
//
// DESCRIPTION:
//
//      This macro routes all of the comparison operators for a class whose
//      operators happen to be defined at the global level.
//
// INPUT PARAMETERS:
//     T1 - The name of the left-hand-side class.
//     T2 - The name of the right-hand-side class.
// 
//*****************************************************************************

#define STL_ROUTE_ALL_GLOBALS(T1, T2) \
namespace std \
{ \
  inline bool operator == ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator == (s1, s2); \
  } \
  inline bool operator != ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator != (s1, s2); \
  } \
  inline bool operator > ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator > (s1, s2); \
  } \
  inline bool operator < ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator < (s1, s2); \
  } \
  inline bool operator >= ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator >= (s1, s2); \
  } \
  inline bool operator <= ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator <= (s1, s2); \
  } \
}

//*****************************************************************************
// STL_DECLARE_GLOBAL_NE(T1, T2) (macro) / Inequality
// STL_DECLARE_GLOBAL_GT(T1, T2) (macro) / Greater than
// STL_DECLARE_GLOBAL_GE(T1, T2) (macro) / Greater than or equal to
// STL_DECLARE_GLOBAL_LE(T1, T2) (macro) / Less than or equal to
//
// DESCRIPTION:
//
//      These macros duplicate the behavior in the STL's function.h file.
//      This behavior provides default implementations of certain comparison
//      operators:  !=, >, >=, and <=.
//
//      STL is designed to instantiate these operators automatically if
//      they are missing.  This convenient feature of the STL has wreaked havoc 
//      on MFC and as a result Microsoft punted and said "use the std namespace."
//      However, this introduced a new problem -- the STL can no longer
//      automatically derive !=, <, >=, and <= from the objects' existing
//      operators, because the operators are not accessible from the std
//      namespace (they are in the global namespace).  In fact, STL cannot
//      even find the default global operators (such as == between two
//      classes).  This seems like a bug in Microsoft's compiler.
//
//      If you are storing a new class of objects in an STL container and
//      want to use find() on the container, then you must declare a global
//      equality operator on the object.  Then you can either create your
//      own != operator, or derive one using STL_DECLARE_GLOBAL_NE. 
//
//      e.g., bool operator == ( const X&, const X& )
//            STL_DECLARE_GLOBAL_NE( X, X )
//
//      It's not a bad idea to declare:
//            bool operator < ( const X&, const X& ) too.
//
//      These macros should be used with classes that implement their
//      comparison operators as global functions.
//
// INPUT PARAMETERS:
//     T1 - The name of the left-hand-side class.
//     T2 - The name of the right-hand-side class.
// 
//*****************************************************************************

// Retuires == (T1, T2)
#define STL_DECLARE_GLOBAL_NE(T1, T2) \
namespace std \
{ \
  inline bool operator != ( const T1 &s1, const T2 &s2 ) \
  { \
    return !( ::operator == (s1, s2) ); \
  } \
}

// Requires < ( T2, T1 )
#define STL_DECLARE_GLOBAL_GT(T1, T2) \
namespace std \
{ \
  inline bool operator > ( const T1 &s1, const T2 &s2 ) \
  { \
    return ::operator < (s2, s1); \
  } \
}

// Requires < ( T1, T2 )
#define STL_DECLARE_GLOBAL_GE(T1, T2) \
namespace std \
{ \
  inline bool operator >= ( const T1 &s1, const T2 &s2 ) \
  { \
    return !( ::operator < (s1, s2) ); \
  } \
}

// Requires < ( T2, T1 )
#define STL_DECLARE_GLOBAL_LE(T1, T2) \
namespace std \
{ \
  inline bool operator <= ( const T1 &s1, const T2 &s2 ) \
  { \
    return !( ::operator < (s2, s1) ); \
  } \
}

//*****************************************************************************
// Route CString's comparison operators.
//*****************************************************************************

STL_ROUTE_ALL_GLOBALS(CString, CString)
