package Translator;

import java.util.Map;

public class StatisticsReport {

	private final String domainName;
	private final String problemName;
	public Map<String, InfoSet> reports;
	
	public StatisticsReport(String domainName, String problemName) {
		
		this.domainName = domainName;
		this.problemName = problemName;
		
	}

}
