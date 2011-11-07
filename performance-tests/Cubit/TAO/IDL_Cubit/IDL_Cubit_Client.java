// $Id$
//
// ============================================================================
//
// = FILENAME
//    IDL_Cubit_Client.java
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// = DESCRIPTION
//   Connects to the cubit factory, gets a cubit object and
//   calls the cube_short method on it. The purpose of this
//   class is to demonstrate how to use the JavaIDL ORB to access
//   TAO.
//
// ============================================================================


public class IDL_Cubit_Client
{

  private org.omg.CORBA.ORB orb_;
  private org.omg.CORBA.Object naming_service_object_;

  public void init (String[] args)
    {
      try {

	orb_ = org.omg.CORBA.ORB.init (args, null);

	// Get the Naming Service initial reference

	System.out.println ("Using the lookup protocol!");
	NS_Resolve ns_resolve = new NS_Resolve ();
	naming_service_object_ = ns_resolve.resolve_name_service (orb_);

      }
      catch(org.omg.CORBA.SystemException e) {
	System.err.println ("PushConsumerFactory constructor: ORB and Name Service initialization");
	System.err.println(e);
      }

    }


  public void run ()
    {
      try
	{

	  // Get the Naming Context to allow resolving the EventService and
	  // ScheduleService
	  CosNaming.NamingContext naming_context =
	    CosNaming.NamingContextHelper.narrow (naming_service_object_);

	  if (naming_context == null)
	    {
	      System.err.println ("The Naming Context is null");
	      System.exit (1);
	    }
	  System.out.println ("Reference to the Naming Service is ok.");

	  CosNaming.NameComponent[] cubit_factory_name = new CosNaming.NameComponent[2];
	  cubit_factory_name[0] = new CosNaming.NameComponent ("IDL_Cubit","");
	  cubit_factory_name[1] = new CosNaming.NameComponent ("cubit_factory","");
	  org.omg.CORBA.Object factory_obj = naming_context.resolve (cubit_factory_name);

	  if (factory_obj == null)
	    {
	      System.err.println ("Factory object is nil!");
	      return;
	    }

       	  Cubit_Factory cubit_Factory = Cubit_FactoryHelper.narrow (factory_obj);

	  Cubit cubit = cubit_Factory.make_cubit ("key0");

	  short x = 4;

	  System.out.println ("The call cube_short (4) results in: " +cubit.cube_short (x));

	}
      catch (CosNaming.NamingContextPackage.CannotProceed e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.CannotProceed");
	  System.err.println (e);
	}
      catch (CosNaming.NamingContextPackage.InvalidName e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.InvalidName");
	  System.err.println (e);
	}
      catch (CosNaming.NamingContextPackage.NotFound e)
	{
	  System.err.println ("CosNaming.NamingContextPackage.NotFound");
	  System.err.println (e);
	}
      catch(org.omg.CORBA.SystemException e)
	{
	  System.err.println ("PushConsumerFactory.run: Failure");
	  System.err.println(e);
	}
    }

  public static void main (String[] args) {

    IDL_Cubit_Client cubit_Client = new IDL_Cubit_Client ();
    cubit_Client.init (args);

    cubit_Client.run ();
  }


} // public class IDL_Cubit_Client







