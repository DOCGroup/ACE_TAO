#include "BON.h"
#include "BONImpl.h"
#include "SchemaLogger.h"

#ifndef SCHEMAVISITOR__H__
#define SCHEMAVISITOR__H__

NSBEGIN

class SchemaVisitor : public BON::Visitor
{
	private:
		SchemaLogger			*logger;
		bool					loginit;
	public :
		SchemaVisitor() {	logger=glob_log;loginit=true;	}
		virtual ~SchemaVisitor() {}

		//void visitObjectImpl(const BON::Object& object);
		void visitModelImpl(const BON::Model& model);
		//void visitFolderImpl( const BON::Folder& folder );
		void visitAtomImpl( const BON::Atom& atom );
		void visitReferenceImpl( const BON::Reference& reference );
		friend	SchemaVisitor& operator<<(SchemaVisitor&,const char*);
		friend	SchemaVisitor& operator<<(SchemaVisitor&,const std::string&);
		friend	SchemaVisitor& operator<<(SchemaVisitor&,const CString&);
		friend	SchemaVisitor& operator<<(SchemaVisitor&,const int&);
		friend	SchemaVisitor& operator<<(SchemaVisitor&,const double&);
};

SchemaVisitor& operator<<(SchemaVisitor&,const char*);
SchemaVisitor& operator<<(SchemaVisitor&,const std::string&);
SchemaVisitor& operator<<(SchemaVisitor&,const CString&);
SchemaVisitor& operator<<(SchemaVisitor&,const int&);
SchemaVisitor& operator<<(SchemaVisitor&,const double&);

NSEND
#endif
