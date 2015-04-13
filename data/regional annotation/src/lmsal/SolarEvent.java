package lmsal;

import java.io.*;
import java.text.*;
import java.util.*;

import com.Httpcom;

public class SolarEvent {
	String path = "SolarEventCount/";
	static String concept[] = {"ar", "ce", "cc", "cd", "ch", "cj", "cr", "cw", "ef",
		"er", "fi", "fe", "fa", "fl", "lp", "os", "sg", "sp", "ss", "to", "pg"};
	
	int id, N, D;
	String days[];
	int events[][];
	boolean isdownloading = false;
	
	SolarEvent(){
		Runtime run=Runtime.getRuntime();//当前 Java 应用程序相关的运行时对象。  
        run.addShutdownHook(new Thread(){ //注册新的虚拟机来关闭钩子  
            public void run() {  
            	if ( !isdownloading ) return ;
                //程序结束时进行的操作
            	saveEvents(path + "events.txt.bak");
                System.out.println("结束前保存了数据");  
            }  
        });
		
	}
	
	String getAPI(String day, int conceptid){
		String api = "";
		String start_time = day+"T00:00:00";
		String end_time = day+"T23:59:59";
		
		api = "http://www.lmsal.com/hek/her?cosec=1&&cmd=search&type=column&event_type="
				+ concept[conceptid]
				+ "&event_region=all&event_coordsys=helioprojective&x1=-5000&x2=5000&y1=-5000&y2=5000&result_limit=1&event_starttime="
				+ start_time
				+ "&event_endtime="
				+ end_time;
		if ( concept[conceptid].compareTo("ch") == 0 ){
			api += "&sparam0=ch.area_atdiskcenter&op0=%3E&value0=24349400000";
		}
		return api;
	}
	
	public static void main(String[] args) throws FileNotFoundException {
		SolarEvent se = new SolarEvent();
		//se.readData();
		//se.genData();
		//se.test();
		//se.runsinglethread();
		se.Merge();
		
		
	}
	
	void runsinglethread(){
		readData();
		isdownloading = true;
		for (int i=0; i<N; i++){
			System.out.println( days[i] +" starting." );
			for (int j=0; j<D; j++){
				if ( events[i][j] != -1 ) continue;
				Httpcom httpcom = new Httpcom();
				byte []res = httpcom.get( getAPI(days[i], j) );
				String str = new String(res);
				if ( !str.contains("html") ){
					System.err.println("network bug: " + i+","+j );
				}else if ( str.contains("result") ){
					events[i][j] = 1;
				}else{
					events[i][j] = 0;
				}
			}
			saveEvents(path + "events.txt");
			saveEvents(path + "events.txt.bak");
		}
	}
	
	void test(){
		for (int i=0; i<concept.length; i++){
			//System.out.println( getAPI("2011-01-01", i) );
			Httpcom httpcom = new Httpcom();
			byte []res = httpcom.get( getAPI("2011-01-01", i) );
			String str = new String(res);
			//System.out.println(str);
			if ( str.contains("result") ){
				System.out.println(concept[i] + " Yes");
			}else{
				System.out.println(concept[i] + " No");
			}
			
		}
		
	}
	
	
	void readData(){
		try {
			N = 0;
			D = concept.length;
			ArrayList<String> daytemp = new ArrayList<String>();
			BufferedReader br = new BufferedReader( new FileReader(path+"days.txt") );
			String str;
			while ( (str=br.readLine()) != null ){
				daytemp.add( str );
				N++;
			}
			br.close();
			
			days = new String[N];
			for ( int i=0; i<N; i++ ){ days[i] = daytemp.get(i); }
			
			///read events
			events = new int[N][D];
			br = new BufferedReader( new FileReader(path+"events.txt") ); 
			for (int i=0; i<N; i++){
				String strs = br.readLine();
				String s[] = strs.split(",");
				for (int j=0; j<D; j++){
					events[i][j] = Integer.parseInt( s[j] );
					//System.out.print(events[i][j]+" ");
				}
				//System.out.println("");
			}
			br.close();
			
			//for ( String t : days ){System.out.println(t);}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		
	}
	
	void genData(){
		N = 365 + 90;
		D = concept.length;
		days = new String[N];
		try {
			BufferedWriter bw;
			bw = new BufferedWriter( new FileWriter(path + "days.txt") );
			Calendar cal = Calendar.getInstance();
			cal.setTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse("2014-01-01 00:00:00"));
			java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-dd");
			for (int i=0; i<N; i++){			
				String day = format.format(cal.getTime()).toString();			
				cal.add(Calendar.DAY_OF_MONTH, 1);
				bw.write(day+"\n");
				days[i] = day;
				System.out.println(day);
			}
			bw.close();
			
			events = new int[N][];
			for (int i=0; i<N; i++){
				events[i] = new int [D];
				for (int j=0; j<D; j++){
					events[i][j] = -1;
				}				
			}
			saveEvents(path + "events.txt");
			
		}
		catch (Exception e1) {
			e1.printStackTrace();
		}	
		
	}
	
	void saveEvents(String fileName ){
		try {
			BufferedWriter bw;
			bw = new BufferedWriter( new FileWriter(fileName) );		
			for (int i=0; i<N; i++){
				for (int j=0; j<D; j++){
					bw.write( ((Integer)(events[i][j])).toString() );
					if ( j==D-1 ) bw.write("\n");else bw.write(",");
				}
			}
			bw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	void Merge(){
		Merge("days.txt");
		Merge("events.txt");
	}
	void Merge(String fileName){
		try{
			String subpaths[] = {"tmp1/", "tmp2/", "tmp3/", "tmp4/"};
			BufferedWriter bw = new BufferedWriter(new FileWriter(path + fileName) );
			for (String subpath : subpaths){
				BufferedReader br = new BufferedReader(
					new FileReader(path + subpath + fileName) );
				String str;
				while( (str = br.readLine()) != null ){
					bw.write( str +"\n" );
				}
				br.close();
			}
			bw.close();
		}catch( Exception e ){
			e.printStackTrace();
		}
	
	}
}
