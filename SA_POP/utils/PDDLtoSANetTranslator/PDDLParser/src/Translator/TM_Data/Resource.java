package Translator.TM_Data;

public class Resource {

	public Resource(String resourceID, String kind, String capacity) {
		this.resourceID = resourceID;
		this.kind = kind;
		this.capacity = capacity;
	}

	private String resourceID;
	private String kind;
	private String capacity;
	
	public String getCapacity() {
		return capacity;
	}

	public String getResourceID(){
		return resourceID;
	}
	
	public String getKind(){
		return kind;
	}
	
	public String toString(){
		return ("ResourceID: "+ resourceID+" Kind: "+kind+" Capacity: "+capacity);
	}
	
}
