// -*-C++-*-
//=====================================================================
/**
 *  @file Process_Element.i
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#include "Process_Element.h"

DOMDocument* create_document (const char *url);

/*
 *  Base class for the process function classes.
 */

template <typename DATA>
class Process_Function {
public:
  virtual void call(DOMDocument*, DOMNodeIterator*, DATA&)=0;

  void operator() (DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    call(doc, iter, data);
  }
};

/*
 *  Wrapper class for the process member functions.
 */

template <typename OBJ, typename DATA>
class Process_Member_Function: public Process_Function<DATA> {
public:
  typedef void (OBJ::*func_type) (DOMNodeIterator*, DATA&);
  typedef DATA data_type;

  Process_Member_Function(OBJ& obj, func_type f)
    : obj_(&obj), f_(f)
  {
  }

  Process_Member_Function(OBJ* obj, func_type f)
    : obj_(obj), f_(f)
  {
  }

  virtual void call(DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    obj_->set_doc (doc);
    obj_->set_iter (iter);
    (obj_->*f_) (iter, data);
  }

private:
  OBJ* obj_;
  func_type f_;
};

/*
 *  Wrapper class for the process member functions which does not have DOMNodeIterator parameter
 */

template <typename OBJ, typename DATA>
class Process_Member_Function_Remote: public Process_Function<DATA> {
public:
  typedef void (OBJ::*func_type) (DATA&);
  typedef DATA data_type;

  Process_Member_Function_Remote(OBJ& obj, func_type f)
    : obj_(&obj), f_(f)
  {
  }

  Process_Member_Function_Remote(OBJ* obj, func_type f)
    : obj_(obj), f_(f)
  {
  }

  virtual void call(DOMDocument* doc, DOMNodeIterator* iter, DATA& data)
  {
    obj_->set_iter(iter);
    obj_->set_doc(doc);
    (obj_->*f_) (data);
  }

private:
  OBJ* obj_;
  func_type f_;
};

/*
 *  Wrapper class for the static process member functions.
 */

template <typename DATA>
class Process_Static_Function: public Process_Function<DATA> {
public:
  typedef void (*func_type) (DOMNodeIterator*, DATA&);
  typedef DATA data_type;

  Process_Static_Function(func_type f)
    : f_(f)
  {
  }

  virtual void call(DOMDocument*, DOMNodeIterator* iter, DATA& data)
  {
    (*f_) (iter, data);
  }

private:
  func_type f_;
};

template <typename SEQUENCE, typename DATA>
inline void
process_sequential_element (DOMNode* node,
                            DOMDocument* doc,
                            DOMNodeIterator* iter,
                            SEQUENCE& seq,
                            Process_Function <DATA>* func,
                            REF_MAP& id_map)
{
  if (node->hasAttributes ())
    {
      // the size of the sequence
      CORBA::ULong i (seq.length ());
      // add 1 to the size of the sequence
      seq.length (i + 1);
      // fetch attributes
      DOMNamedNodeMap* named_node_map = node->getAttributes ();
      // the number of attributes the element have
      int length = named_node_map->getLength();
      // if there is no other attribute but 'version'

      ACE_DEBUG ((LM_DEBUG, "i am inside seq element\n"));

      if (length == 1) // call directly the static process_ method
        (*func) (doc, iter, seq[i]);
      else             // Check the xmi::id & href attributes
        process_element_attributes(named_node_map, doc, iter, i, seq[i], func, id_map);
    }
}

/*
 *  Process function for member functions - id_map: obj->id_map()
 */

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence_local(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                       XStr& node_name, const char* name,
                       SEQUENCE& seq, OBJECT* obj, FUNCTION func)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      Process_Member_Function<OBJECT, DATA>
        pf(obj, func);
      process_sequential_element (node, doc, iter, seq, &pf, obj->id_map());
    }

  return result;
}

/*
 *  Process function for member functions -
 */

template<typename DATA, typename OBJECT, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
  ACE_DEBUG ((LM_DEBUG, "i am inside seq remote \n"));
      OBJECT obj (doc, iter, false);
      
      Process_Member_Function_Remote<OBJECT, DATA>
        pf(obj, func);
      process_sequential_element (node, doc, iter, seq, &pf, id_map);
    }

  return result;
}

/*
 *  Process function for static functions
 */

template<typename DATA, typename SEQUENCE, typename FUNCTION>
inline bool
process_sequence_common(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                        XStr& node_name, const char* name,
                        SEQUENCE& seq, FUNCTION func,
                        REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      Process_Static_Function<DATA>
        pf(func);
      process_sequential_element (node, doc, iter, seq, &pf, id_map);
    }

  return result;
}

void
process_refs(DOMNode*& node,
             CORBA::ULongSeq& seq,
             int& index,
             IDREF_MAP& idref_map);

void
process_ref(DOMNode*& node,
            CORBA::ULong& ref,
            int& index,
            IDREF_MAP& idref_map);

/*
 *  Process function for references
 */

inline bool
process_reference_seq(DOMNode* node,
                      XStr& node_name, const char* name,
                      CORBA::ULongSeq& seq,
                      int& index,
                      IDREF_MAP& idref_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
      process_refs (node, seq, index, idref_map);

  return result;
}

// Process reference
inline bool
process_reference (DOMNode* node,
                   XStr& node_name, const char* name,
                   CORBA::ULong& ref,
                   int& index,
                   IDREF_MAP& idref_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));
  
  if (result == true)
    process_ref (node, ref, index, idref_map);

  return result;
}


/*
 *  Process function for non-sequential elements
 */

template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
inline bool
process_element(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                XStr& node_name, const char* name,
                ELEMENT& elem, OBJECT* obj, FUNCTION func,
                REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();

          Process_Member_Function<OBJECT, DATA>
            pf(obj, func);

          if (length == 1)
              pf(doc, iter, elem);
          else
              process_element_attributes(named_node_map, doc, iter, 0, elem, &pf, id_map);
        }
    }

  return result;
}

// Process function for non-sequential non-local elements
template<typename DATA, typename OBJECT, typename ELEMENT, typename FUNCTION>
inline bool
process_element_remote(DOMDocument* doc, DOMNodeIterator* iter, DOMNode* node,
                       XStr& node_name, const char *name,
                       ELEMENT& elem, FUNCTION func,
                       REF_MAP& id_map)
{
  bool result = (node_name == XStr (ACE_TEXT (name)));

  if (result == true)
    {
      OBJECT obj (iter, false);

      if (node->hasAttributes ())
        {
          DOMNamedNodeMap* named_node_map = node->getAttributes ();
          int length = named_node_map->getLength ();

          Process_Member_Function_Remote<OBJECT, DATA>
            pf(&obj, func);

          if (length == 1)
              pf(doc, iter, elem);
          else
              process_element_attributes(named_node_map, doc, iter, 0, elem, &pf, id_map);
        }
    }

  return result;
}

