package com;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.*;

public class Httpcom {
	/**
	 * 用get方法，获得url的二进制数据
	 */
	public byte[] get(String url_str)
	{
		byte ret[] = null;
		boolean success = false;
		while( !success ){
			try{
				URL url = new URL(url_str);
				HttpURLConnection connection = (HttpURLConnection) url.openConnection();
				connection.setConnectTimeout(5000);
				connection.setReadTimeout(36000);
				connection.setInstanceFollowRedirects(true);
				connection.setRequestProperty("accept", "*/*");
		        connection.setRequestProperty("connection", "Keep-Alive");
		        connection.setRequestProperty("user-agent",
		        		"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;SV1)");
				connection.connect();
				
				InputStream inputStream = connection.getInputStream();
				ArrayList<Byte> v = new ArrayList<Byte>();
				byte []buf = new byte[65536];
				int len = -1;
		        while ( (len = inputStream.read(buf) ) != -1 ) {
		            //System.out.println(len);
		            for (int i=0; i<len; i++){
		            	v.add(buf[i]);
		            }
		        }
		        ret = new byte[ v.size() ];
		        for (int i=v.size()-1; i>=0; i--){
		        	ret[i] = v.get(i);
		        }
		        inputStream.close();
		        connection.disconnect();
		        success = true;
			}catch (Exception e) {
		        System.out.println("http get Exception! "+e.getMessage() + "; Try again!");
		        success = false;
		        try {
					Thread.sleep(3000);
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				}
		    }
		}
		return ret;
	}
	public static void main(String av[]) throws IOException{
		Httpcom t = new Httpcom();
		//byte []s = t.get("http://bt.neu6.edu.cn");
		byte []s = t.get("http://[2001:778:0:ffff:64:0:80b7:a824]/assets/img/browse/2011/08/09/20110809_081041_4096_0094.jpg");
//		File file = new File("test.jpg");
//		FileOutputStream fos = new FileOutputStream( file );
//		fos.write(s);
//		fos.close();
		System.out.println(s.length);
	}
}
