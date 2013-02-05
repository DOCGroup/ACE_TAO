// -*- C++ -*-

//=============================================================================
/**
 * @file  Storable_Naming_Context_ReaderWriter.cpp
 *
 * $Id$
 *
 * @author Marina Spivak <marina@cs.wustl.edu>
 * @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Naming/Storable_Naming_Context_ReaderWriter.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Storable.h"

#include "tao/Storable_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Storable_Naming_Context_ReaderWriter::
TAO_Storable_Naming_Context_ReaderWriter (TAO::Storable_Base & stream)
  : stream_(stream)
{
}

void
TAO_Storable_Naming_Context_ReaderWriter::write (TAO_Storable_Naming_Context & context)
{
  TAO_NS_Persistence_Header header;

  header.size (static_cast<unsigned int> (context.storable_context_->current_size()));
  header.destroyed (context.destroyed_);

  this->write_header(header);

  if (0u == header.size ())
    return;

  ACE_Hash_Map_Iterator<TAO_Storable_ExtId,TAO_Storable_IntId,
                        ACE_Null_Mutex> it = context.storable_context_->map().begin();
  ACE_Hash_Map_Iterator<TAO_Storable_ExtId,TAO_Storable_IntId,
                        ACE_Null_Mutex> itend = context.storable_context_->map().end();

  ACE_Hash_Map_Entry<TAO_Storable_ExtId,TAO_Storable_IntId> ent = *it;

  while (!(it == itend))
  {
    TAO_NS_Persistence_Record record;

    ACE_CString name;
    CosNaming::BindingType bt = (*it).int_id_.type_;
    if (bt ==  CosNaming::ncontext)
      {
        CORBA::Object_var
          obj = context.orb_->string_to_object ((*it).int_id_.ref_.in ());
        if (obj->_is_collocated ())
          {
            // This is a local (i.e. non federated context) we therefore
            // store only the ObjectID (persistence filename) for the object.

            // The driving force behind storing ObjectIDs rather than IORs for
            // local contexts is to provide for a redundant naming service.
            // That is, a naming service that runs simultaneously on multiple
            // machines sharing a file system. It allows multiple redundant
            // copies to be started and stopped independently.
            // The original target platform was Tru64 Clusters where there was
            // a cluster address. In that scenario, clients may get different
            // servers on each request, hence the requirement to keep
            // synchronized to the disk. It also works on non-cluster system
            // where the client picks one of the redundant servers and uses it,
            // while other systems can pick different servers. (However in this
            // scenario, if a server fails and a client must pick a new server,
            // that client may not use any saved context IORs, instead starting
            // from the root to resolve names. So this latter mode is not quite
            // transparent to clients.) [Rich Seibel (seibel_r) of ociweb.com]

            PortableServer::ObjectId_var
              oid = context.poa_->reference_to_id (obj.in ());
            CORBA::String_var
              nm = PortableServer::ObjectId_to_string (oid.in ());
            const char
              *newname = nm.in ();
            name.set (newname); // The local ObjectID (persistance filename)
            record.type (TAO_NS_Persistence_Record::LOCAL_NCONTEXT);
          }
        else
          {
            // Since this is a foreign (federated) context, we can not store
            // the objectID (because it isn't in our storage), if we did, when
            // we restore, we would end up either not finding a permanent
            // record (and thus ending up incorrectly assuming the context was
            // destroyed) or loading another context altogether (just because
            // the contexts shares its objectID filename which is very likely).
            // [Simon Massey  (sma) of prismtech.com]

            name.set ((*it).int_id_.ref_.in ()); // The federated context IOR
            record.type (TAO_NS_Persistence_Record::REMOTE_NCONTEXT);
          }
      }
    else // if (bt == CosNaming::nobject) // shouldn't be any other, can there?
      {
        name.set ((*it).int_id_.ref_.in ()); // The non-context object IOR
        record.type (TAO_NS_Persistence_Record::OBJREF);
      }
    record.ref(name);

    const char *myid = (*it).ext_id_.id();
    ACE_CString id(myid);
    record.id(id);

    const char *mykind = (*it).ext_id_.kind();
    ACE_CString kind(mykind);
    record.kind(kind);

    write_record (record);
    it.advance();
  }

  context.write_occurred_ = 1;
}

int
TAO_Storable_Naming_Context_ReaderWriter::read (TAO_Storable_Naming_Context & context)
{
  // assume file already open for reading
  TAO_Storable_Bindings_Map *bindings_map;

  // create the new bindings map
  ACE_NEW_THROW_EX (bindings_map,
                    TAO_Storable_Bindings_Map (context.hash_table_size_, context.orb_.in()),
                    CORBA::NO_MEMORY ());

  // get the data for this bindings map from the file

  TAO_NS_Persistence_Header header;
  TAO_NS_Persistence_Record record;

  // we are only using the size from this header
  this->read_header(header);

  // reset the destroyed flag
  context.destroyed_ = header.destroyed();

  // read in the data for the map
  for (unsigned int i= 0u; i<header.size(); ++i)
    {
      this->read_record(record);

      if (TAO_NS_Persistence_Record::LOCAL_NCONTEXT == record.type ())
        {
          PortableServer::ObjectId_var
            id = PortableServer::string_to_ObjectId (record.ref ().c_str ());
          const char
            *intf = context.interface_->_interface_repository_id ();
          CORBA::Object_var
            objref = context.poa_->create_reference_with_id (id.in (), intf);
          bindings_map->bind ( record.id ().c_str (),
                               record.kind ().c_str (),
                               objref.in (),
                               CosNaming::ncontext );
        }
      else
        {
          CORBA::Object_var
            objref = context.orb_->string_to_object (record.ref ().c_str ());
          bindings_map->bind ( record.id ().c_str (),
                               record.kind ().c_str (),
                               objref.in (),
                               ((TAO_NS_Persistence_Record::REMOTE_NCONTEXT == record.type ())
                                ? CosNaming::ncontext    // REMOTE_NCONTEXT
                                : CosNaming::nobject )); // OBJREF
        }
    }
  context.storable_context_ = bindings_map;
  context.context_ = context.storable_context_;
  return 0;
}

void
TAO_Storable_Naming_Context_ReaderWriter::write_header (const TAO_NS_Persistence_Header & header)
{
  stream_.rewind();
  stream_ << header.size();
  stream_ << header.destroyed();
  stream_.flush();
}
void
TAO_Storable_Naming_Context_ReaderWriter::read_header (TAO_NS_Persistence_Header & header)
{
  unsigned int size;
  int destroyed;

  stream_.rewind();

  stream_ >> size;
  header.size(size);

  stream_ >> destroyed;
  header.destroyed(destroyed);
}

void
TAO_Storable_Naming_Context_ReaderWriter::write_record (const TAO_NS_Persistence_Record & record)
{
  TAO_NS_Persistence_Record::Record_Type type = record.type();
  stream_ << type;

  stream_ << record.id();
  stream_ << record.kind();
  stream_ << record.ref();

  stream_.flush();
}

void
TAO_Storable_Naming_Context_ReaderWriter::read_record (TAO_NS_Persistence_Record & record)
{
  int temp_type_in;
  stream_ >> temp_type_in;
  TAO_NS_Persistence_Record::Record_Type type =
    (TAO_NS_Persistence_Record::Record_Type) temp_type_in;
  record.type (type);

  ACE_CString record_id;
  stream_ >> record_id;
  record.id (record_id);

  ACE_CString record_kind;
  stream_ >> record_kind;
  record.kind (record_kind);

  ACE_CString record_ref;
  stream_ >> record_ref;
  record.ref (record_ref);
}

void
TAO_Storable_Naming_Context_ReaderWriter::write_global (const TAO_NS_Persistence_Global & global)
{
  stream_.rewind();
  stream_ << global.counter();
  stream_.flush();
}

void
TAO_Storable_Naming_Context_ReaderWriter::read_global (TAO_NS_Persistence_Global
                                                       & global)
{
  unsigned int counter = 0;

  stream_.rewind();
  // We expect an exception to be thrown with EOF state if the file is empty.
  try
    {
      stream_ >> counter;
    }
  catch (TAO::Storable_Read_Exception &ex)
    {
      if (ex.get_state() != TAO::Storable_Base::goodbit &&
          ex.get_state() != TAO::Storable_Base::eofbit)
        throw CORBA::INTERNAL ();
    }

  global.counter(counter);

}

TAO_END_VERSIONED_NAMESPACE_DECL
