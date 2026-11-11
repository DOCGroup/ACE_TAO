// -*- C++ -*-
//=============================================================================
/**
 * @file    id_map.hpp
 *
 * This file contains the ID_Map class which links XML IDREF objects to their
 * XML ID counterparts. In the output files, this means that a <IDREF
 * name>_ptr() call returns a pointer to the identified object.
 *
 * NULL_PTR_Entry is thrown when a null pointer is added to either map.
 * Unresolved_IDREF is thrown when an XML document references an ID that does
 * not exist.
 *
 * @author Jason R. Cody <jason.r.cody@vanderbilt.edu; jason.r.cody@gmail.com>
 */
//=============================================================================

#ifndef _ID_MAP_H
#define _ID_MAP_H

#include "ace/TSS_T.h"
#include "ace/ace_wchar.h"
#include "ace/XML_Utils/XMLSchema/Types.hpp"

#include <map>
#include <string>

/**
 * @class ID_Map
 * @brief Maps XML IDs and IDREFs to their corresponding XML objects.
 *
 * The stored XSCRT::Type pointers are non-owning. The parsed XML object graph
 * owns those objects and must outlive this map.
 */
class ID_Map
{
public:
  using TSS_ID_Map = ACE_TSS<ID_Map>;
  using String = std::basic_string<ACE_TCHAR>;
  using ID_MAP = std::map<String, XSCRT::Type *>;
  using IDREF_MAP = std::multimap<String, XSCRT::Type *>;
  using id_iterator = ID_MAP::const_iterator;
  using idref_iterator = IDREF_MAP::const_iterator;

  class NULL_PTR_Entry {};

  class Unresolved_IDREF
  {
  public:
    explicit Unresolved_IDREF (String const &message)
      : message_ (message)
    {
    }

    String const &get_message () const
    {
      return this->message_;
    }

  private:
    String message_;
  };

  ID_Map () = default;
  ID_Map (ID_Map const &) = delete;
  ID_Map &operator= (ID_Map const &) = delete;
  ID_Map (ID_Map &&) = delete;
  ID_Map &operator= (ID_Map &&) = delete;

  void add_id (String const &id, XSCRT::Type *obj_ref)
  {
    if (obj_ref != nullptr)
    {
      this->id_map_.try_emplace (id, obj_ref);
    }
    else
    {
      throw NULL_PTR_Entry ();
    }
  }

  void add_idref (String const &idref, XSCRT::Type *obj_ref)
  {
    if (obj_ref != nullptr)
    {
      this->idref_map_.emplace (idref, obj_ref);
    }
    else
    {
      throw NULL_PTR_Entry ();
    }
  }

  void resolve_single_idref (String const &idref, XSCRT::Type *element) const
  {
    ID_MAP::const_iterator const id_iter = this->id_map_.find (idref);
    if (id_iter != this->id_map_.end ())
    {
      element->set_idref (idref, id_iter->second);
    }
    else
    {
      throw Unresolved_IDREF (idref);
    }
  }

  void resolve_idref () const
  {
    for (IDREF_MAP::value_type const &idref_entry : this->idref_map_)
    {
      ID_MAP::const_iterator const id_iter = this->id_map_.find (idref_entry.first);
      if (id_iter != this->id_map_.end ())
      {
        idref_entry.second->set_idref (id_iter->first, id_iter->second);
      }
      else
      {
        throw Unresolved_IDREF (idref_entry.first);
      }
    }
  }

  void reset ()
  {
    this->id_map_.clear ();
    this->idref_map_.clear ();
  }

private:
  // Both maps borrow pointers from the parsed XML object graph.
  ID_MAP id_map_;
  IDREF_MAP idref_map_;
};

#endif /* _ID_MAP_H */
