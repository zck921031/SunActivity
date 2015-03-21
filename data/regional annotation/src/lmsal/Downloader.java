package lmsal;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.file.Files;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class Downloader {
	//访问lmsal的api
	public void Query(String start_time, String end_time){
		String filename = "data/"+start_time.substring(0, 10)+".xml";
		if ( ( new File(filename) ).exists() ) {
			System.out.println(filename + " is exists, processing ignored");
			return ;
		}

		System.out.println("processing "+filename);
		String res = "";
		String ask = "http://www.lmsal.com/hek/her?cosec=1&&cmd=search&type=column&event_type=ch,fl,ss&event_region=all&event_coordsys=helioprojective&x1=-5000&x2=5000&y1=-5000&y2=5000"
				+ "&event_starttime="+start_time+"&event_endtime="+end_time
				+ "&sparam0=ch.area_atdiskcenter&op0=%3E&value0=608735000";
		try{
			String url_str = ask;
			URL url = new URL(url_str);
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			connection.setConnectTimeout(20000);
			connection.setReadTimeout(20000);
			//connection.setInstanceFollowRedirects(true);
			connection.setRequestProperty("accept", "*/*");
            connection.setRequestProperty("connection", "Keep-Alive");
            connection.setRequestProperty("user-agent",
            		"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
			connection.connect();
			
            BufferedReader in = new BufferedReader(new InputStreamReader(
                    connection.getInputStream(), "UTF-8" ) );
            String line;
            StringBuffer tmp = new StringBuffer();
            while ((line = in.readLine()) != null) {
            	tmp.append( line+"\n" );
                //System.out.println(line);
            }
            res = tmp.toString();
            if (in != null) in.close();
            connection.disconnect();
		}catch (Exception e) {
            System.out.println("lmsal Exception on Query! "+e.getMessage());
            return ;
        }
		save(res, "data/"+start_time.substring(0, 10)+".xml");
	}
	private void save(String res, String file) {		
		//System.out.println(file);
		try {
			BufferedWriter bw = new BufferedWriter( new OutputStreamWriter( new FileOutputStream(file)));
			bw.write(res);
			bw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}	
	public static void main(String[] args) throws ParseException {
		//设置开始日期
		Calendar cal = Calendar.getInstance();
		cal.setTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse("2011-08-06 00:00:00"));
		java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss"); 
		//System.out.println("The Time is:"+format.format(cal.getTime()) );
		
		Downloader downloader = new Downloader(); 
		for (int i=0; i<800; i++){
			String start_time = format.format(cal.getTime()).toString();
			cal.add(Calendar.DAY_OF_MONTH, 1);
			String end_time = format.format(cal.getTime()).toString();
			start_time = start_time.replaceAll(" ", "T");
			end_time = end_time.replaceAll(" ", "T");

			downloader.Query(start_time, end_time);
		}
	}

}
