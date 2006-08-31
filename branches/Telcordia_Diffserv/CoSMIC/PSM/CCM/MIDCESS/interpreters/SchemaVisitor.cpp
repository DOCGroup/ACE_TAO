#include "SchemaVisitor.h"

NSBEGIN

void SchemaVisitor::visitModelImpl( const BON::Model& model )
{
	*this<<"Visited model object "<<model->getName()<<" \n";
	std::string otype = model->getObjectMeta().name();
	//*this<<" ModelImpl has meta-type "<< otype << "\n";
	if (otype == "Domain")
	{
		BON::Attribute attr = model->getAttribute("UUID");
		//*this <<" Domain UUID was : " << attr->getStringValue() << " \n";
	}
	if (otype == "Bridge")
	{
		//BON::Attribute attr = model->getAttribute("UUID");
		//*this <<" Domain UUID was : " << attr->getStringValue() << " \n";
	}


}

void SchemaVisitor::visitReferenceImpl( const BON::Reference& reference )
{
	*this<<"Visited Reference object "<<reference->getName()<<" \n";
}

void SchemaVisitor::visitAtomImpl( const BON::Atom& atom )
{
	*this<<"Visited Atom object "<<atom->getName()<<" \n";
}

SchemaVisitor& operator<<(SchemaVisitor& gv,const char* cptr)
{
	if(gv.loginit)
		*gv.logger<<CString(cptr);
	return gv;
}

SchemaVisitor& operator<<(SchemaVisitor& gv,const std::string& s)
{
	if(gv.loginit)
		*gv.logger<<s;
	return gv;
}

SchemaVisitor& operator<<(SchemaVisitor& gv,const CString& s)
{
	if(gv.loginit)
		*gv.logger<<s;
	return gv;
}

SchemaVisitor& operator<<(SchemaVisitor& gv,const int& i)
{
	if(gv.loginit)
		*gv.logger<<i;
	return gv;
}

SchemaVisitor& operator<<(SchemaVisitor& gv,const double& f)
{
	if(gv.loginit)
		*gv.logger<<f;
	return gv;
}

NSEND
