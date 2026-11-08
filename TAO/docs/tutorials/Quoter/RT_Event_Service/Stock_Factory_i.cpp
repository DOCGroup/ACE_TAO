
#include "Stock_Factory_i.h"
#include "Stock_i.h"
#include "tao/Utils/PolicyList_Destroyer.h"

#include <iostream>
#include <limits>
#include <string>

Quoter::Stock_ptr
Quoter_Stock_Factory_i::get_stock (char const *symbol)
{
  PortableServer::ObjectId_var oid (PortableServer::string_to_ObjectId (symbol));

  try
    {
      CORBA::Object_var tmp (this->stock_factory_poa_->id_to_reference (oid.in ()));
      return Quoter::Stock::_narrow (tmp.in ());
    }
  catch (PortableServer::POA::ObjectNotActive const &)
    {
      throw Quoter::Invalid_Stock_Symbol ();
    }
}

void
Quoter_Stock_Factory_i::destroy_stock_objects ()
{
  if (CORBA::is_nil (this->stock_factory_poa_.in ()))
    return;

  this->stock_factory_poa_->destroy (1, 1);
  this->stock_factory_poa_ = PortableServer::POA::_nil ();
}

void
Quoter_Stock_Factory_i::load_stock_objects (
  PortableServer::POA_ptr poa,
  PortableServer::POAManager_ptr poa_manager,
  RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  if (!CORBA::is_nil (this->stock_factory_poa_.in ()))
    return;

  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  policies[0] =
    poa->create_id_assignment_policy (PortableServer::USER_ID);
  policies[1] =
    poa->create_implicit_activation_policy (PortableServer::NO_IMPLICIT_ACTIVATION);

  this->stock_factory_poa_ =
    poa->create_POA ("Stock_Factory_POA",
                     poa_manager,
                     policies);

  std::string symbol;
  std::string full_name;
  double price {};
  while (std::getline (std::cin >> std::ws, symbol) &&
         std::getline (std::cin, full_name) &&
         std::cin >> price)
    {
      std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

      Quoter_Stock_i *const stock = new Quoter_Stock_i (symbol,
                                                        full_name,
                                                        price);
      // The POA servant var owns the stock until the POA takes its reference.
      PortableServer::ServantBase_var servant (stock);
      PortableServer::ObjectId_var oid (
        PortableServer::string_to_ObjectId (symbol.c_str ()));

      this->stock_factory_poa_->activate_object_with_id (oid.in (),
                                                         servant.in ());
      stock->connect (supplier_admin);
    }
}
