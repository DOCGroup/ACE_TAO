package Translator.TM_Data;

public class TaskToImpl {

	public TaskToImpl(String taskID, String implID, String duration) {
		this.taskID = taskID;
		this.implID = implID;
		this.duration = duration;
	}

	private String taskID;
	private String implID;
	private String duration;
	
	public String getTaskID(){
		return taskID;
	}
	
	public String getImplID(){
		return implID;
	}
	
	public String getDuration(){
		return duration;
	}
	
	public String toString(){
		return ("TaskID: "+ " ImplID: "+implID+" Duration: "+duration);
	}
}