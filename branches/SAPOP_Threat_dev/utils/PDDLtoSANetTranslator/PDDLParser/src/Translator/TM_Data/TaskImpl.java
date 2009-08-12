package Translator.TM_Data;

public class TaskImpl {
	
	public TaskImpl(String p_implID){
		implID = p_implID;
		param = new Param();
	}
	
	private String implID;
	private Param param;
	
	public String getImplID(){
		return implID;
	}
	
	public Param getParam(){
		return param;
	}
	
	public String toString(){
		return ("ImplID: "+implID);
	}

	public class Param{
		
		String paramID = "test_param1";
		String kind = "test_kind1";
		String value = "test_param_value1";
		
		public String getParamID(){
			return paramID;
		}
		
		public String getKind(){
			return kind;
		}
		
		public String getValue(){
			return value;
		}
	}
}

