// $Id$

//=============================================================================
/**
 *  @file $Name$
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef CIAO_ASSEMBLY_SPEC_H
#define CIAO_ASSEMBLY_SPEC_H

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Containers_T.h"
#include "ace/SString.h"

namespace CIAO
{
  /**
   * @typedef ID_IMPL_MAP
   *
   * A hash map type for indexing implmentation IDs to corresponding
   * softpkg paths.
   */
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ACE_CString,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> ID_IMPL_MAP;

  namespace Assembly_Placement
  {
    /**
     * @enum Node Type
     */
    typedef enum _nodetype
    {
      INVALID_NODE,
      INVALID_CONTAINER,
      HOST,
      PROCESS,
      HOME,
      COMPONENT
    } Node_Type;

    // Forward declaration.
    class Visitor;

    /**
     * @class Node
     *
     * Abstract base class for placement information
     */
    class Node
    {
    public:
      /// Default constructor.
      Node (const char *id = 0);

      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      virtual ~Node ();

      void usagename (const char *un);
      const char *usagename (void) const;

      const char *id (void) const;

      /// Double linked list required internal data
      Node *prev_;
      Node *next_;

    protected:
      /// Node ID.
      ACE_CString id_;

      /// Usage information.
      ACE_CString usagename_;
    };

    /**
     * @class Container
     *
     * Abstract base class for container type placement node
     */
    class Container : public Node,
                      public ACE_Double_Linked_List<Node>
    {
    public:
      Container (const char *id,
                 unsigned long cardinality);

      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      //@{
      /** Accesor/mutator for destination information */
      void destination (const char *des);
      const char *destination (void) const;

      unsigned long cardinality (void) const;
      //@}

    protected:
      /// A corbaloc string pointing to a CIAO_Daemon interface.
      ACE_CString destination_;

      // cardinality
      unsigned long cardinality_;
    };

    /**
     * @class componentinstantiation
     */
    class componentinstantiation : public Node
    {
    public:
      typedef enum _if_register_type
        {
          COMPONENT,
          PROVIDESID,
          CONSUMESID
        } IF_Register_Type;

      typedef enum _register_method
        {
          NAMINGSERVICE,
          IORFILE               // CIAO extension
        } Register_Method;

      typedef struct _register_info
      {
        IF_Register_Type type_;
        Register_Method method_;

        ACE_CString port_id_;
        ACE_CString name_;

        void reset ()
        {
          type_ = COMPONENT;
          port_id_.clear ();
          name_.clear ();
        }
      } Register_Info;

      componentinstantiation (const char *id);


      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      virtual ~componentinstantiation ();

      ACE_Unbounded_Queue <Register_Info> register_info_;

    protected:
    };

    /**
     * @class homeplacement
     */
    class homeplacement : public Container
    {
    public:
      typedef enum _register_method
        {
          HOMEFINDER,
          NAMING,
          TRADER                // No implementation for trader yet.
        } Register_Method;

      typedef struct _register_info
      {
        /// Register_Method
        Register_Method type_;

        /// Name to be registered with the finder/namingservice
        ACE_CString name_;
      } Register_Info;

      homeplacement (const char *id,
                     unsigned long cardinality = 1);
      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      virtual ~homeplacement ();

      //@{
      /** Accessor/mutator functions  */
      void componentfileref (const char *file);
      const char *componentfileref (void) const;
      //@}

      ACE_Unbounded_Queue <Register_Info> register_info_;

    protected:
      // idref to component implementation file.
      ACE_CString componentfileref_;
    };

    /**
     * @class hostcollocation
     */
    class hostcollocation : public Container
    {
    public:
      hostcollocation (const char *id,
                       unsigned long cardinality = 1);

      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      virtual ~hostcollocation ();

    protected:
    };

    /**
     * @class hostcollocation
     */
    class processcollocation : public Container
    {
    public:
      processcollocation (const char *id,
                          unsigned long cardinality = 1);

      /// Accepting a visitor.
      virtual int accept (Visitor &v);

      virtual ~processcollocation ();

    protected:
    };

    /**
     * @class Placement_Visitor
     *
     * Abstract Base class for placement node visitor.
     */
    class Visitor
    {
    public:
      Visitor ();

      virtual ~Visitor () = 0;

      virtual int visit_Container (Container *c) = 0;

      virtual int visit_hostcollocation (hostcollocation *hc) = 0;

      virtual int visit_processcollocation (processcollocation *pc) = 0;

      virtual int visit_homeplacement (homeplacement *hp) = 0;

      virtual int visit_componentinstantiation (componentinstantiation *ci) = 0;

    protected:
    };
  }

  namespace Assembly_Connection
  {
    /**
     *
     */
    typedef enum _if_resolution
      {
        PROVIDER,               // Requires a component ref. (compound)
        CONSUMER,               // Requires a component ref. (compound)
        COMP_IDREF,             // Requires an idref
        HOME_IDREF,             // Requires an idref
        NAMINGSERVICE,          // Requires a name
        STRINGIFIEDOBJECTREF,   // Requires an IOR
        HOMEFINDER,             // Requries a name (similar to naming)
        TRADERQUERY             // Require trader query structure.  No support for now.
      } IF_Resolution_Method;

    typedef enum _conxion_type
      {
        INTERFACE,
        EMITTER_CONSUMER,
        PUBLISHER_CONSUMER,
        HOME,
        INVALID_CONN
      } Connection_Type;

    class IF_Resolver_Info
    {
    public:
      IF_Resolver_Info (IF_Resolution_Method type,
                        const char *info,
                        IF_Resolver_Info *nested = 0);

      ~IF_Resolver_Info ();

      IF_Resolution_Method resolver_type (void) const;

      /// The string we use to resolve the interface.
      const char *resolver_info (void) const;

      /// Return a nested resolver this resolver depends on.
      IF_Resolver_Info *nested_resolver (void);

      /// Return the trader structure.  (Not implemented yet.
      /// therefore, we are returning void * for now.)
      void *traderquery (void) const;

    protected:
      /// Hints the kind of resolve info.
      IF_Resolution_Method resolver_type_;

      /// Information this info contains
      ACE_CString resolver_info_;

      /// Dependent resolver info.
      IF_Resolver_Info *nested_resolver_;

      /// @@@ Future placeholder for trader query info.
      void *traderquery_;
    };

    /**
     *
     */
    typedef struct _CII
    {
      _CII ();

      ~_CII ();

      // The kind of connection this info describes.
      Connection_Type type_;

      /// The id of the connection.
      ACE_CString id_;

      /// The name of the receptacle or consumer.
      ACE_CString name_;

      /// The component that uses the receptacle or consumes the event.
      IF_Resolver_Info *component_;

      /// The interface that is to be connected to the port.
      IF_Resolver_Info *interface_;

    } Connect_Info;
  }

  /**
   *
   */
  typedef struct _ASpec
  {
    _ASpec ();

    /// ID to implementation map.
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            ACE_CString,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> componentfiles_;

    /// Partitioning data
    Assembly_Placement::Container partitioning_;

    /// Connection data
    ACE_Unbounded_Queue<Assembly_Connection::Connect_Info*> connections_;
  } Assembly_Spec;
}

#if defined (__ACE_INLINE__)
# include "Assembly_Spec.inl"
#endif /* __ACE_INLINE__ */
#endif /* CIAO_ASSEMBLY_SPEC_H */
