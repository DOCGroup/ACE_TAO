//*****************************************************************************
//
// Class Ptr
//
// Ptr is a proxy for a pointer to an object.  This class provides automatic
// deletion of objects allocated on the heap.  Objects are viewed via the
// ->, *, and (itemClass *) operators.  Since these operators are inline, there
// is no performance penalty for using this proxy instead of a real pointer.
//
// You can assign multiple pointers to one Ptr object.  If you
// do, the old object will be destroyed automatically.
//
// Use suggestions:
//  1. To clean up pointers returned by functions or methods that  
//     return pointers and expect the caller to delete the pointer.
//  2. To make sure an object gets deleted.  Using Ptr saves you
//     the hassle of writing an exception handler for this purpose.
//  3. Part of a composite object (assembly-part).
//     This class is a useful substitute when pointers are needed,
//     because object clean-up is fully encapsulated.
//     The following are cases where pointers would normally be used.
//
//     For declaring class data members that may or
//     may not be instantiated (1 to 0-or-1 relationship). 
//
//     To instantiate a data member only when it is really needed, 
//     reducing average memory consumption during execution.
//
//     MFC and thread-local storage -- some MFC objects can only be
//     accessed by the thread that created them.  MFC sub-component
//     objects cannot be instantiated along with its container object
//     if the container object is created in one thread and the MFC 
//     object is accessed in another.
//
//     When a 'part' class does not have a default constructor.
//     There are two solutions provided by C++ for this situation.
//     The designer could use a pointer to the object instead, 
//     or the designer could alter the assembly class's constructor 
//     to accept parameters that are passed on to the 'part' class's
//     constructor.
//
// See Design Patterns - Proxy pattern, and the Iterator pattern (pg 266).
//
// Template Parameters:
//   itemClass: The class of the underlying object (the 'pointed to' object).
//
//*****************************************************************************

template <class itemClass>
class Ptr
{
  public:

    // Construction
    Ptr( itemClass *pItem = NULL );
    Ptr( const Ptr& );

    // Destruction
    ~Ptr();
    
    // Access to 'real' object
    inline itemClass * operator -> () const;
    inline operator itemClass * () const;
    inline itemClass& operator * () const;

    // Assignment
    Ptr& operator = ( const Ptr& );
    Ptr& operator = ( itemClass *pItem );
    itemClass *& GetPtrRef();
    
    // Other
    void Disown();

  private:

    itemClass *m_pObject;
};

//*****************************************************************************
//
// Ptr default constructor
//
// DESCRIPTION:
//
//      Constructor. 
//
// INPUT PARAMETERS:
//      pItem - Pointer to the object to point to.  pItem is deleted
//              when 'this' is deleted.
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>::Ptr<itemClass>( itemClass *pItem )
  : m_pObject( pItem )
{
}

//*****************************************************************************
//
// Ptr::Ptr (Ptr&)
//
// DESCRIPTION:
//
//      Copy constructor. To avoid deleting 'rCopy's object 
//      twice, 'rCopy' will not point to any object after this method completes. 
//      rCopy is not really const, but it is declared 'const' since
//      this allows putting Ptr's in STL containers.
//
// INPUT PARAMETERS:
//      rCopy - A reference to the Ptr to copy.
//
// OUTPUT PARAMETERS:
//      rCopy - No longer points to anything.
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>::Ptr<itemClass>( const Ptr<itemClass>& rCopy )
{
  m_pObject = rCopy.m_pObject;
  
  // rCopy no longer owns the object.
  const_cast< Ptr<itemClass>& >(rCopy).m_pObject = NULL;
}

//*****************************************************************************
//
// Ptr::~Ptr
//
// DESCRIPTION:
//
//      Destroys the object that is being pointed to (if any).
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>::~Ptr<itemClass>()
{
  delete m_pObject; // delete NULL OK
}

//*****************************************************************************
//
// Ptr::GetPtrRef
//
// DESCRIPTION:
//      
//      Returns a reference to an internal pointer.  This allows these 
//      objects to be sent to functions that accept pointers to pointers
//      or references to pointers as output arguments.  Typically, this
//      is how functions return multiple objects. 
// 
//      This is not const because this method is ultimately intended
//      for changing the object's value.
//
// RETURNS:
//      A reference to the 'pointer' data member of this object.
//
//*****************************************************************************

template <class itemClass>
itemClass *& Ptr<itemClass>::GetPtrRef()
{ 
  return m_pObject; 
}

//*****************************************************************************
//
// Ptr::operator ->
//
// DESCRIPTION:
//      
//      Provides access to the interface of the underlying object.
//
// RETURNS:
//      Nothing callers can really use - only the compiler can use it.
//      This method is part of the pointer to member operator (-> *).
//
//*****************************************************************************

template <class itemClass>
itemClass * Ptr<itemClass>::operator -> () const
{ 
  return m_pObject; 
}

//*****************************************************************************
//
// Ptr::operator itemClass * 
//
// DESCRIPTION:
//      
//      Provides access to the underlying object.
//
// RETURNS:
//      Pointer to the object that is being pointed to.
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>::operator itemClass * () const
{ 
  return m_pObject; 
}

//*****************************************************************************
//
// Ptr::operator * 
//
// DESCRIPTION:
//      
//      Provides access to the underlying object.
//
// RETURNS:
//      Reference to the object that is being pointed to.
//
//*****************************************************************************

template <class itemClass>
itemClass& Ptr<itemClass>::operator * () const
{ 
  return *m_pObject; 
}

//*****************************************************************************
//
// Ptr::operator = (Ptr&)
//
// DESCRIPTION:
//
//      For assigning one Ptr to another.  Deletes the object that 'this' 
//      is pointing to and makes 'this' point to the object that 'rCopy'
//      is pointing to.  To avoid deleting 'rCopy's object twice, 'rCopy'
//      will not point to any object after this method completes. 
//
//      Although the parameter rCopy can be arugably not "const", it
//      must be "const" since the compiler will complain in certain
//      cases with warning C4270. 
//
// INPUT PARAMETERS:
//      rCopy - A reference to the Ptr to copy.
//
// OUTPUT PARAMETERS:
//      rCopy - No longer points to anything.
//
// RETURNS:
//      A reference to 'this'.
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>&
Ptr<itemClass>::operator = ( const Ptr<itemClass>& rCopy )
{
  // Check for A = A 
  if ( &rCopy == this )
  {
    return *this;
  } 
  
  // Save current pointer so we can delete it after
  // doing everything else
  itemClass *pOldObject = m_pObject;
  
  // Get the pointer out of rCopy
  m_pObject = rCopy.m_pObject;
    
  // rCopy no longer owns the object.
  const_cast< Ptr<itemClass>& >(rCopy).m_pObject = NULL;
  
  // This might generate an exception.  But, we won't
  // introduce a memory leak because 'this' now
  // owns rCopy's pointer.
  delete pOldObject; // delete NULL OK
  
  return *this;
}

//*****************************************************************************
//
// Ptr::operator =
//
// DESCRIPTION:
//
//      Changes the underlying object.  If the proxy currently has an
//      underlying object, then it is deleted.
//
// INPUT PARAMETERS:
//      pItem - Reference to the new object.  Can be NULL.
//
// RETURNS:
//      Reference to 'this'.
//
//*****************************************************************************

template <class itemClass>
Ptr<itemClass>& Ptr<itemClass>::operator = ( itemClass *pItem )
{
  if ( m_pObject == pItem )
  {
    return *this;
  }
  
  // Save current pointer so we can delete it after
  // doing everything else
  itemClass *pOldObject = m_pObject;
  
  m_pObject = pItem;
  
  // This might generate an exception.  But, we won't
  // introduce a memory leak because 'this' now
  // owns pItem. 
  delete pOldObject; // delete NULL OK
  
  return *this;
}

//*****************************************************************************
//
// Ptr::Disown 
//
// DESCRIPTION:
//
//      Ptr objects delete their pointed-to objects when they go out
//      of scope.  Calling Disown() causes the Ptr object to 'forget' 
//      that it is currently pointing to an object.
//
// RETURNS:
//      Doesn't return anything because returning a value such as a pointer
//      to the item encourages misuse leading to memory leaks.
// 
//*****************************************************************************
template <class itemClass>
void Ptr<itemClass>::Disown()
{
  m_pObject = NULL;
}
