// $Id$

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::Node::Node (const char *id)
  : prev_ (0),
    next_ (0),
    id_ (id)
{
}

ACE_INLINE void
CIAO::Assembly_Placement::Node::usagename (const char *un)
{
  this->usagename_ = un;
}

ACE_INLINE const char *
CIAO::Assembly_Placement::Node::usagename () const
{
  return this->usagename_.c_str ();
}

ACE_INLINE const char *
CIAO::Assembly_Placement::Node::id () const
{
  return this->id_.c_str ();
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::Container::Container (const char *id,
                                                unsigned long cardinality)
  : CIAO::Assembly_Placement::Node (id),
    cardinality_ (cardinality)
{
}

ACE_INLINE void
CIAO::Assembly_Placement::Container::destination (const char *des)
{
  this->destination_ = des;
}

ACE_INLINE const char *
CIAO::Assembly_Placement::Container::destination (void) const
{
  if (this->destination_.length () == 0)
    return 0;
  return this->destination_.c_str ();
}

ACE_INLINE unsigned long
CIAO::Assembly_Placement::Container::cardinality (void) const
{
  return this->cardinality_;
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::componentinstantiation::componentinstantiation (const char *id)
  : CIAO::Assembly_Placement::Node (id)
{
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::homeplacement::homeplacement (const char *id,
                                                        unsigned long cardinality)
  : CIAO::Assembly_Placement::Container (id,
                                         cardinality)
{
}

ACE_INLINE const char *
CIAO::Assembly_Placement::homeplacement::componentfileref (void) const
{
  return this->componentfileref_.c_str ();
}

ACE_INLINE void
CIAO::Assembly_Placement::homeplacement::componentfileref (const char *ref)
{
  this->componentfileref_ = ref;
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::hostcollocation::hostcollocation (const char *id,
                                                            unsigned long cardinality)
  : CIAO::Assembly_Placement::Container (id, cardinality)
{
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::processcollocation::processcollocation (const char *id,
                                                                  unsigned long cardinality)
  : CIAO::Assembly_Placement::Container (id, cardinality)
{
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Placement::Visitor::Visitor (void)
{
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Connection::IF_Resolver_Info::IF_Resolver_Info
(CIAO::Assembly_Connection::IF_Resolution_Method type,
 const char *info,
 CIAO::Assembly_Connection::IF_Resolver_Info *nested)
  : resolver_type_ (type),
    resolver_info_ (info),
    nested_resolver_ (nested),
    traderquery_ (0)
{
}

ACE_INLINE
CIAO::Assembly_Connection::IF_Resolver_Info::~IF_Resolver_Info (void)
{
  delete this->nested_resolver_;
}

ACE_INLINE CIAO::Assembly_Connection::IF_Resolution_Method
CIAO::Assembly_Connection::IF_Resolver_Info::resolver_type (void) const
{
  return this->resolver_type_;
}

ACE_INLINE const char *
CIAO::Assembly_Connection::IF_Resolver_Info::resolver_info (void) const
{
  return this->resolver_info_.c_str ();
}

ACE_INLINE CIAO::Assembly_Connection::IF_Resolver_Info *
CIAO::Assembly_Connection::IF_Resolver_Info::nested_resolver (void)
{
  return this->nested_resolver_;
}

ACE_INLINE void *
CIAO::Assembly_Connection::IF_Resolver_Info::traderquery (void) const
{
  return this->traderquery_;
}

// ================================================================

ACE_INLINE
CIAO::Assembly_Connection::_CII::_CII (void)
  : type_ (CIAO::Assembly_Connection::INVALID_CONN),
    component_ (0),
    interface_ (0)
{
}

ACE_INLINE
CIAO::Assembly_Connection::_CII::~_CII (void)
{
  // Do not manage memory here.  We will try to manage the memory
  // explicitly for now.
#if 0
  delete this->component_;
  delete this->interface_;
#endif /* 0 */
}

// ================================================================

ACE_INLINE
CIAO::_ASpec::_ASpec (void)
  : partitioning_ ("CIAO_ROOT_PARTITIONING",
                   0)

{
}
