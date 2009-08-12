package Translator.TM_Data;

public class ImplToResource {

	public ImplToResource(String implID, String resourceID, String utilization) {
		this.implID = implID;
		this.resourceID = resourceID;
		this.utilization = utilization;
	}

	private String implID;
	private String resourceID;
	private String utilization;
	
	public String getImplID(){
		return implID;
	}
	
	public String getResourceID(){
		return resourceID;
	}
	
	public String getUtilization(){
		return utilization;
	}
	
	public String toString(){
		return ("ImplID: "+ implID+ " ResourceID: "+resourceID+ " Utilization: "+ utilization);
	}
	
}
