/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Parse_Node.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_PARSE_NODE_H)
#define ACE_PARSE_NODE_H

#include "ace/Service_Record.h"

class ACE_Export ACE_Parse_Node
  // = TITLE
  //    Provide the base of the object hierarchy that defines the parse
  //    tree of Service Nodes.
{
public:
  ACE_Parse_Node (void);
  ACE_Parse_Node (const char *name);
  virtual ~ACE_Parse_Node (void);

  ACE_Parse_Node *link (void) const;
  void link (ACE_Parse_Node *);
  virtual void apply (void) = 0;

  const char *name (void) const;
  void print (void) const;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const char *name_;
  ACE_Parse_Node *next_;
};

class ACE_Export ACE_Suspend_Node : public ACE_Parse_Node
  // = TITLE
  //     Suspend a Service Node.
{
public:
  ACE_Suspend_Node (const char *name);

  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Resume_Node : public ACE_Parse_Node
  // = TITLE
  //     Resume a Service Node.
{
public:
  ACE_Resume_Node (const char *name);

  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Remove_Node : public ACE_Parse_Node
  // = TITLE
  //     Remove a Service Node.
{
public:
  ACE_Remove_Node (const char *name);

  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

class ACE_Export ACE_Static_Node : public ACE_Parse_Node
  // = TITLE
  //     Handle a statically linked node.
{
public:
  ACE_Static_Node (const char *name, char *params = 0);
  virtual ~ACE_Static_Node (void);

  virtual void apply (void);
  virtual const ACE_Service_Record *record (void) const;
  char *parameters (void) const;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  char *parameters_;
};

class ACE_Export ACE_Dynamic_Node : public ACE_Static_Node
  // = TITLE
  //     Handle a dynamically linked node.
{
public:
  ACE_Dynamic_Node (const ACE_Service_Record *, char *params);
  virtual ~ACE_Dynamic_Node (void);

  virtual const ACE_Service_Record *record (void) const;
  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Service_Record *record_;
};

class ACE_Export ACE_Stream_Node : public ACE_Parse_Node
  // = TITLE
  //     Handle a Stream.
{
public:
  ACE_Stream_Node (const ACE_Static_Node *, const ACE_Parse_Node *);
  virtual ~ACE_Stream_Node (void);

  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Static_Node *node_;
  const ACE_Parse_Node  *mods_;
};

class ACE_Export ACE_Location_Node
  // = TITLE
  //     Keep track of where a shared library is located.
{
public:
  ACE_Location_Node (void);
  virtual const void *symbol (void) = 0;
  virtual void set_symbol (const void *h);
  const void *handle (void) const;
  void handle (const void *h);
  const char *pathname (void) const;
  void pathname (const char *h);
  int dispose (void) const;

  virtual ~ACE_Location_Node (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  const void *open_handle (void);
			    
  const void *handle_;
  const void *symbol_;
  const char *pathname_;
  int	must_delete_;
};

class ACE_Export ACE_Object_Node : public ACE_Location_Node
  // = TITLE
  //   Keeps track of the symbol name for a shared object.
{
public:
  ACE_Object_Node (const char *pathname, const char *obj_name);
  virtual const void *symbol (void);
  virtual ~ACE_Object_Node (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const char *object_name_;
};

class ACE_Export ACE_Function_Node : public ACE_Location_Node
  // = TITLE
  //     Keeps track of the symbol name of for a shared function.
{
public:
  ACE_Function_Node (const char *pathname, const char *func_name);
  virtual const void *symbol (void);
  virtual ~ACE_Function_Node (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const char *function_name_;
};

class ACE_Export ACE_Dummy_Node : public ACE_Parse_Node
  // = TITLE
  //     I forget why this is here... ;-)
{
public:
  ACE_Dummy_Node (const ACE_Static_Node *, const ACE_Parse_Node *);
  ~ACE_Dummy_Node (void);
  virtual void apply (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Static_Node *node_;
  const ACE_Parse_Node *mods_;
};

#if defined (__ACE_INLINE__)
#include "ace/Parse_Node.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PARSE_NODE_H */
