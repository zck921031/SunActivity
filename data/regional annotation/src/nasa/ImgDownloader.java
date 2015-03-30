package nasa;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.atomic.AtomicLong;
import java.util.regex.*;

import com.Httpcom;


public class ImgDownloader {
	static String imgpath = "img/";
	static TreeSet<String> imgnames = new TreeSet<String>();  

	
	public void solve_one_day(String date){
		list_img_name(date);

		
	} 
	
	/**
	 *  从nasa网站获取图片名字
	 */
	public Set<String> list_img_name(String date)
	{
		Set<String> ret = new TreeSet<String>();
		String year, month, day;
		String[] str_tmp = date.split("-");
		year = str_tmp[0]; month = str_tmp[1]; day = str_tmp[2];
		String url_str = "http://sdo.gsfc.nasa.gov/assets/img/browse/"+year+"/"+month+"/"+day+"/";
		//System.out.println(url_str);

		boolean success = false;
		while( !success ){
			StringBuffer res = new StringBuffer();
			try{
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
		        while ((line = in.readLine()) != null) {
		        	res.append( line );
		            //System.out.println(line);
		        }
		        if (in != null) in.close();
		        connection.disconnect();
		        success = true;
			}catch (Exception e) {
		        System.out.println("list name Exception! "+e.getMessage() + "; Try again!");
		        success = false;
		    }finally{
		    	if ( !success ) continue;
				Pattern p=Pattern.compile("\\d{8}_\\d{6}_4096_\\d{4}.jpg");
				Matcher m=p.matcher(res);
				while(m.find()) {
					ret.add( m.group() ); 
				}
		    }
		}
		return ret;
	}

	/**
	 * 由文件和网络获取N天的图片名字，放入imgnames
	 * @param N
	 * @throws ParseException
	 */
	public void get_all_filename(int N) throws ParseException{
		HashSet<String> dates = new HashSet<String>();
		File f = new File("NASA_img_name.txt");
		if ( f.exists() ){
			System.out.println("reading some filenames from NASA_img_name.txt");
			try
			{
				BufferedReader br = new BufferedReader( new InputStreamReader( new FileInputStream( f ) ) );
				String str;
				while( ( str=br.readLine() ) !=null ){
					imgnames.add(str);
					String date = str.split("_")[0];
					dates.add(date);
				}
				br.close();
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}else{
			try
			{
				f.createNewFile();
			}
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}

		//设置开始日期
		Calendar cal = Calendar.getInstance();
		cal.setTime(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").parse("2011-08-06 00:00:00"));
		java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("yyyy-MM-dd");
		
		for (int i=0; i<N; i++){
			String date = format.format(cal.getTime()).toString();
			
			if ( dates.contains( date.replaceAll("-", "") ) ){
				//System.out.println(date + " has been read from txt.");
			}else{
				System.out.println("Processing " + date);
				
				Set<String>res = list_img_name(date);
				Iterator<String> it = res.iterator();
				StringBuffer sb = new StringBuffer();
				while ( it.hasNext() ){
					String t = it.next();
					sb.append( t + "\n" );
					imgnames.add(t);
					//System.out.println( t );
				}
				writeFile("NASA_img_name.txt", sb.toString(), true );
			}
			cal.add(Calendar.DAY_OF_MONTH, 1);
		}
		System.out.println( imgnames.size() );
	}
	
	/**
	 * 根据照片的时间，将照片聚成每组9张图片的SunFrame
	 * @return ArrayList<SunFrame>
	 */
	ArrayList<SunFrame> genSunFrames(){		
		ArrayList<String> names = new ArrayList<String>();
		for (String t : imgnames){
			String strs[] = t.split("[_,.]");
			if ( strs.length != 5 ) continue;
			if ( strs[3].compareTo("4500") == 0 ) continue;
			names.add(t);
		}
		
		ArrayList<SunFrame> sunFrames = new ArrayList<SunFrame>(); 
		int len = names.size();
		for ( int i=0; i<len; i++ ){
			int cnt = 0, j = i;
			SunFrame sunFrame = new SunFrame(); 
			while( cnt<9 && j<len ){
				String t = names.get(j);
				sunFrame.add(t);
				cnt++;	j++;
			}
			if ( sunFrame.check() ){
				sunFrames.add( sunFrame );
				i = i+8;
			}
		}
		System.out.println( "imgnames size: " + names.size() );
		System.out.println( "sunFrames size: " + sunFrames.size() );
		return sunFrames;
	}
	
	/**
	 * 由lmsal的标记和已有图片，生成所需要的图片信息
	 * @param N
	 * @return
	 */
	ArrayList<SunFrame> get_need_img(int N){
		ArrayList<SunFrame> ret = new ArrayList<SunFrame>();
		try
		{
			//read annotation
			File file = new File("../../Code/demo1/demo1/lmsal.txt");
			BufferedReader br = new BufferedReader( new InputStreamReader( new FileInputStream( file ) ) );
			String str;
			ArrayList<Anno> annos = new ArrayList<Anno>(); 
			while( ( str=br.readLine() ) !=null ){
				Anno t = new Anno();
				t.set(str);
				annos.add(t);
			}
			br.close();
			
			System.out.println( annos.size() );
			ArrayList<Anno> flare = new ArrayList<Anno>();
			HashMap<Long,Long> hm = new HashMap<Long,Long>();
			SimpleDateFormat lmsalformat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Calendar cal = Calendar.getInstance();		
			for ( Anno t : annos ){
				if ( t.concept.compareTo("Coronal Hole") != 0 ) continue;
				if ( t.event_endtime.compareTo("2013-01-01T00:00:00") > 0 ) continue;
				//flare area
				//if ( t.area > 3e5 || t.area < 5e4 ) continue;
				//Sunspot area
				//if ( t.area > 3e5 || t.area < 5e3 ) continue;
				//Coronal Hole area
				if ( t.area > 3e5 || t.area < 1e5 ) continue;

				if ( t.event_peaktime==null || t.event_peaktime.length()<2 ){
					if ( t.event_starttime == null || t.event_starttime.length()<2 ) continue;
					t.event_peaktime = t.event_starttime;
				}
				long key = t.get_poskey();
				cal.setTime( lmsalformat.parse(t.event_starttime.replaceAll("T", " ") ) );
				long value = cal.getTimeInMillis();
				if ( hm.containsKey(key) ){
					if ( Math.abs( hm.get(key) - value ) < 1000*60*60*24 )	continue;
				}
				hm.put(key, value);

				flare.add( t );
				//t.print();	if ( flare.size()>30 ) break;
			}
			hm=null;
			annos=null;
			System.out.println( flare.size() );			
			
//			for (Anno t : flare){
//				if ( t.event_starttime.compareTo( t.event_endtime ) == 0 ){
//					System.err.println("Flare: time sequence bug occurred!");
//				}
//			}
		
			ArrayList<SunFrame> sunFrames = genSunFrames();

			ArrayList<String> eve0 = new ArrayList<String>();
			ArrayList<String> eve1 = new ArrayList<String>();
			for (Anno t : flare){
				eve0.add( t.event_starttime );
				eve1.add( t.event_endtime );
			}
			Collections.sort(eve0);
			Collections.sort(eve1);
			
			Collections.sort(flare, new Comparator<Anno>(){
				public int compare(Anno x, Anno y){
					return x.event_peaktime.compareTo( y.event_peaktime );
				}
			});

			int idx0=0, idx1=0, n=flare.size();
			for (SunFrame t : sunFrames){
//				while( idx0<n && eve0.get(idx0).compareTo( t.time1 ) <= 0 ) idx0++;
//				while( idx1<n && eve1.get(idx1).compareTo( t.time0 ) < 0 ) idx1++;
//				t.hit = idx0 - idx1;

				while( idx0<n && flare.get(idx0).event_peaktime.compareTo( t.starttime ) < 0 ) idx0++;
				while( idx1<n && flare.get(idx1).event_peaktime.compareTo( t.endtime ) <= 0 ) idx1++;
				for (int i=idx0; i<idx1; i++){
					Anno p = flare.get(i);
					double weight =	Math.max( 
							Math.max( Math.abs(p.left), Math.abs(p.right) ),
							Math.max( Math.abs(p.up), Math.abs(p.down) ) );
					weight = 1.0 - 1.0 / (1.0 + Math.exp( -(weight - 850.0)/100.0 ) );									
					t.hit += p.area * weight;
				}
			}

			Collections.sort( sunFrames,new Comparator<SunFrame>(){
	            public int compare(SunFrame x, SunFrame y) {
	                return ( x.hit > y.hit ? -1 : x.hit < y.hit ? 1 : 0);
	            }
	        });
			
			int cnt = 0;
			for (SunFrame t : sunFrames){
				if ( t.hit <= 0 ) break;
				if ( ++cnt >= 65 ) break;
				ret.add( t );
			}
			System.out.println("need " + cnt + " pictures.");
//			for (SunFrame t : ret){
//				 System.out.println( t.starttime + " " + t.hit );
//			}
			
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return ret;
	}
	
	void saveAnnotation(){
		try
		{
			//read annotation
			File file = new File("../../Code/demo1/demo1/lmsal.txt");
			BufferedReader br = new BufferedReader( new InputStreamReader( new FileInputStream( file ) ) );
			String str;
			ArrayList<Anno> annosorg = new ArrayList<Anno>(); 
			while( ( str=br.readLine() ) !=null ){
				Anno t = new Anno();
				t.set(str);
				annosorg.add(t);
			}
			br.close();
			
			System.out.println( "annos size : " + annosorg.size() );
			ArrayList<Anno> annos = new ArrayList<Anno>();
			HashMap<Long,Long> hm = new HashMap<Long,Long>();
			SimpleDateFormat lmsalformat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
			Calendar cal = Calendar.getInstance();		
			for ( Anno t : annosorg ){
				if ( t.event_endtime.compareTo("2013-01-01T00:00:00") > 0 ) continue;

				if ( t.area > 2e6 || t.area < 1e4 ) continue;

				if ( t.concept.compareTo("Flare") == 0 && t.area < 3e4 ) continue;
				if ( t.concept.compareTo("Coronal Hole") == 0 && t.area < 1e5 ) continue;

				if ( t.event_peaktime==null || t.event_peaktime.length()<2 ){
					if ( t.event_starttime == null || t.event_starttime.length()<2 ) continue;
					t.event_peaktime = t.event_starttime;
				}
				long key = t.get_poskey();
				cal.setTime( lmsalformat.parse(t.event_starttime.replaceAll("T", " ") ) );
				long value = cal.getTimeInMillis();
				if ( hm.containsKey(key) ){
					if ( Math.abs( hm.get(key) - value ) < 1000*60*60*24 )	continue;
				}
				hm.put(key, value);

				annos.add( t );
				//t.print();	if ( flare.size()>30 ) break;
			}

			System.out.println( "after filter : " + annos.size() );			
			
			ArrayList<SunFrame> sunFrames = genSunFrames();
			
			Collections.sort(annos, new Comparator<Anno>(){
				public int compare(Anno x, Anno y){
					return x.event_peaktime.compareTo( y.event_peaktime );
				}
			});
			Collections.sort(sunFrames, new Comparator<SunFrame>(){
				public int compare(SunFrame x, SunFrame y){
					return x.starttime.compareTo( y.starttime );
				}
			});

			writeFile("anno_nasa_lmsal.txt","#img1,img2,...,img9,{Flare,Coronal Hole,Sunspot},"
					+ "left,up,right,down,\n",false);
			int cnt = 0;
			int idx0=0, idx1=0, n=annos.size();
			for (SunFrame t : sunFrames){
//				while( idx0<n && annos.get(idx0).event_peaktime.compareTo( t.starttime ) < 0 ) idx0++;
//				while( idx1<n && annos.get(idx1).event_peaktime.compareTo( t.endtime ) <= 0 ) idx1++;
				
				if ( !t.exist() ) continue;
				for (int i=0; i<n; i++){
					Anno p = annos.get(i);
					if ( p.event_starttime.compareTo( t.endtime ) > 0 ) continue;
					if ( p.event_endtime.compareTo( t.starttime ) < 0 ) continue;
					if ( p.event_peaktime == "2011-08-09T11:55:00" ){
						System.out.println( p.left + "," + p.up + "," + p.right + "," + p.down );
					}
					cnt++;
					String outstr = "";
					for (String name : t.getnames() ){
						outstr += name + ",";
					}
					outstr += p.concept + ",";
					outstr +=p.left + "," + p.up + "," + p.right + "," + p.down + ",";
					outstr += "\n";
					writeFile("anno_nasa_lmsal.txt", outstr, true);
				}
			}


			System.out.println("write " + cnt + " annotations.");
			
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

	}
	
	void Downloadimg(int N){

		ArrayList<SunFrame> sunFrames = get_need_img(N);
		ArrayList<String> names = new ArrayList<String>(); 
		for ( SunFrame tmp : sunFrames ){
			for ( String name : tmp.getnames() ){
				names.add(name);
			}
		}

		Object b = new Object();
		MDowns test = new MDowns(names, b);
		for (int i=0; i<4; i++){
			new Thread(test).start();
		}
		synchronized (b){
			try {
				b.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("finish~");
		}
	}
	
	void test(){
		ArrayList<String> t0 = new ArrayList<String>();
		t0.add("1");
		t0.add("2");
		t0.add("3");
		t0.add("4");
		t0.add("5");
		t0.add("6");
		t0.add("7");
		Object b = new Object();
		MDowns test = new MDowns(t0, b);
		for (int i=0; i<4; i++){
			new Thread(test).start();
		}
		synchronized (b){
			try {
				b.wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("finish~");
		}
	}
	
	
	public static void main(String[] args) throws ParseException {
		
		ImgDownloader t = new ImgDownloader();
		
		//t.test();
		
		t.get_all_filename(120);
		
		//t.Downloadimg(120);
		
		t.saveAnnotation();

	}
	
	private static void writeFile(String path, String outputStr, boolean append)
	{
	    try
	    {
	        File file = new File(path);
	        BufferedWriter writer = new BufferedWriter(new FileWriter(file, append));
	        writer.write( outputStr );
		    writer.flush();
		    writer.close();
		}
		catch(Exception e)
		{
		    e.printStackTrace();
		}
	}

	
	class SunFrame{
		String names[] = new String[9];
		String time0, time1;
		String starttime, endtime;
		int hit = 0;
		int size = 0;
		long tmin=Long.MAX_VALUE, tmax=Long.MIN_VALUE;
		void add(String name){
			if ( size<9 ){
				names[size++] = name;
			}
		}
		public String[] getnames(){
			return names;
		}
		public boolean exist(){
			for (String name : names){
				if ( !(new File(imgpath+name).exists()) ) return false;
			}
			return true;
		}
		public boolean check(){
			if ( size < 9 ) return false;
			Calendar cal = Calendar.getInstance();			 
			try {
				SimpleDateFormat nasaformat = new SimpleDateFormat("yyyyMMdd_HHmmss");
				SimpleDateFormat lmsalformat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
				for (int i=0; i<9; i++){
					cal.setTime( nasaformat.parse(names[i]) );
					long tmp = cal.getTimeInMillis();
					tmin = Math.min(tmin, tmp);
					tmax = Math.max(tmax, tmp);
				}
				if ( tmax - tmin > 8*60*1000 ) return false;
				time0 = lmsalformat.format( tmin ).toString().replaceAll(" ", "T");
				time1 = lmsalformat.format( tmax ).toString().replaceAll(" ", "T");
				starttime = lmsalformat.format( tmin - 8*60*1000 ).toString().replaceAll(" ", "T");
				endtime = lmsalformat.format( tmax + 8*60*1000 ).toString().replaceAll(" ", "T");
			} catch (ParseException e) {
				e.printStackTrace();
				return false;
			}
			Arrays.sort(names, new Comparator<String>(){
				public int compare(String x, String y){
					return x.substring(x.length()-8, x.length()-4).compareTo(
							y.substring(y.length()-8, y.length()-4) );
				}
			});
			return true;
		}
	}
	
	class Anno{
		public String event_starttime, event_peaktime, event_endtime, concept;
		public double left, right, up, down, area;
		void set(String str){
			String []tmp = str.split(",");
			event_starttime = tmp[0];
			event_peaktime = tmp[1];
			event_endtime = tmp[2];
			concept = tmp[3];
			left = Double.parseDouble(tmp[4]);
			right = Double.parseDouble(tmp[5]);
			up = Double.parseDouble(tmp[6]);
			down = Double.parseDouble(tmp[7]);
			area = (right-left)*(up-down);
		}
		void print(){
			System.out.println( event_starttime );
			System.out.println( event_peaktime );
			System.out.println( event_endtime );
			System.out.println( concept );
			System.out.println( left );
			System.out.println( right );
			System.out.println( up );
			System.out.println( down );
			System.out.println( area );
			System.out.println( get_poskey() );
			System.out.println( "" );
		}
		long get_poskey(){
			long sum = 0;
			sum = sum*4096 + ( 2048 + (long)left );
			sum = sum*4096 + ( 2048 + (long)right );
			sum = sum*4096 + ( 2048 + (long)up );
			sum = sum*4096 + ( 2048 + (long)down ); 
			return sum;
		}
	}
	
	class MDowns implements Runnable{
		ArrayList<String> names;
		AtomicLong i;
		AtomicLong b;
		long size;
		Object flag;
		public MDowns(ArrayList<String> Names, Object Flag) {
			names = Names;
			flag = Flag;
			size = names.size();
			b = new AtomicLong(0);
			i = new AtomicLong(0);
		}
		public void run(){
			b.addAndGet(1);
			while ( true ){
				int idx = (int)i.getAndAdd(1);
				synchronized (flag){
					if ( idx >= size ){
						long cnt = b.addAndGet(-1);
						if ( cnt <= 0 ) flag.notify();
						return ;
					}
				}
				//do download
				String name = names.get(idx);
				String url =
						"http://[2001:778:0:ffff:64:0:80b7:a824]/assets/img/browse/"
						//"http://sdo.gsfc.nasa.gov/assets/img/browse/"
						+name.substring(0, 4)+"/"+name.substring(4, 6)+"/"+name.substring(6, 8)+"/"+name;
				
				File file = new File("img/"+name);
				//don't download again
				if ( file.exists() ) continue;

				System.gc();
				System.out.println(url);
				Httpcom httpcom = new Httpcom();
				byte []img = httpcom.get(url);
				FileOutputStream fos;
				try {
					fos = new FileOutputStream( file );
					fos.write(img);
					fos.close();
				} catch (Exception e) {
					e.printStackTrace();
				}

				try {
					Thread.sleep(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}


