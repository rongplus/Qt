package com.sharkversion.rong.sharkapp;

import android.widget.BaseAdapter;
import  android.view.*;
import  android.widget.*;
import android.content.*;
import java.util.*;
import android.view.View.OnClickListener;

import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.graphics.BitmapFactory;
import java.io.InputStream;
import java.net.URL;
import android.net.Uri;
import android.graphics.Rect;

import 	android.graphics.Matrix;
import org.json.JSONObject;

/**
 * Created by rong on 7/10/17.
 */

public class ListViewAdapter extends BaseAdapter {

    private List<Map<String, Object>> data;
    private LayoutInflater layoutInflater;
    private Context context;
    public ListViewAdapter(Context context,List<Map<String, Object>> data){
        this.context=context;
        this.data=data;
        this.layoutInflater=LayoutInflater.from(context);
    }
    /**
     * 组件集合，对应list.xml中的控件
     * @author Administrator
     */
    public final class Zujian{
        public ImageView image;
        public TextView title;
        public Button view;
        public TextView info;
        public JSONObject pro;
    }
    @Override
    public int getCount() {
        return data.size();
    }
    /**
     * 获得某一位置的数据
     */
    @Override
    public Object getItem(int position) {
        return data.get(position);
    }
    /**
     * 获得唯一标识
     */
    @Override
    public long getItemId(int position) {
        return position;
    }

    public View getView(final int position, View convertView, ViewGroup parent)
    {
        Zujian zujian=null;
        if(convertView==null){
            zujian=new Zujian();
            //获得组件，实例化组件
            convertView=layoutInflater.inflate(R.layout.listview, null);
            zujian.image=(ImageView)convertView.findViewById(R.id.image);
            zujian.title=(TextView)convertView.findViewById(R.id.title);
            zujian.view=(Button)convertView.findViewById(R.id.viewDetail);
            zujian.info=(TextView)convertView.findViewById(R.id.info);


            convertView.setTag(zujian);
        }else{
            zujian=(Zujian)convertView.getTag();
        }
        zujian.pro = new JSONObject();
        //绑定数据
        //new DownLoadImageTask(zujian.image).execute((String) data.get(position).get("image"));
        new DownLoadImageTask(zujian.image).execute("http://sharknet.somee.com/images/head.png");
       // zujian.image.setBackgroundResource((Integer)data.get(position).get("image"));
        //Uri imgUri=Uri.parse("http://sharknet.somee.com/images/head.png");
        //zujian.image.setImageURI( imgUri);
        zujian.title.setText((String)data.get(position).get("title"));
        zujian.info.setText((String)data.get(position).get("info"));
        zujian.pro = (JSONObject)  data.get(position).get("Provider");

        //ViewHolder viewHolder;
        Button btn = (Button) convertView.findViewById(R.id.viewDetail);
        btn.setOnClickListener(new OnClickListener() {


            public void onClick(View v) {
                if (customListner != null) {
                    customListner.onButtonClickListner(position, (String)data.get(position).get("title"));
                }

            }
        });

        return convertView;
    }

    public void addItem(String name,String tel, String wechart,String pic,JSONObject obj)
    {
        Map<String, Object> map=new HashMap<String, Object>();
        map.put("image", pic);
        map.put("title", name);
        map.put("info", wechart);
        map.put("Provider",obj);
        data.add(map);
/*
        map=new HashMap<String, Object>();
        map.put("image", pic);
        map.put("title", "这是一个标题"+0);
        map.put("info", "这是一个详细信息" + 0);

        data.add(map);
*/
    }

    private customButtonListener customListner;
    public interface customButtonListener
    {
        public void onButtonClickListner(int position,String value);
    }

    public void setCustomButtonListner(customButtonListener listener) {
        this.customListner = listener;
    }


    private class DownLoadImageTask extends AsyncTask<String,Void,Bitmap>{
        ImageView imageView;

        public DownLoadImageTask(ImageView imageView){
            this.imageView = imageView;
        }

        /*
            doInBackground(Params... params)
                Override this method to perform a computation on a background thread.
         */
        protected Bitmap doInBackground(String...urls){
            String urlOfImage = urls[0];
            Bitmap logo = null;
            try{
                InputStream is = new URL(urlOfImage).openStream();
                /*
                    decodeStream(InputStream is)
                        Decode an input stream into a bitmap.
                 */
                BitmapFactory.Options options = new BitmapFactory.Options();
                Bitmap resizedBitmap = BitmapFactory.decodeStream(is);

                int width = resizedBitmap.getWidth();
                int height = resizedBitmap.getHeight();

                float scaleWidth = ((float) 48) / width;
                float scaleHeight = ((float) 48) / height;

                // CREATE A MATRIX FOR THE MANIPULATION
                Matrix matrix = new Matrix();
                // RESIZE THE BIT MAP
                matrix.postScale(scaleWidth, scaleHeight);

                // "RECREATE" THE NEW BITMAP
                logo = Bitmap.createScaledBitmap(resizedBitmap, 48, 48, true);//Bitmap.createBitmap( resizedBitmap, 0, 0, 48, 48, matrix, false);
                //logo = resizedBitmap;
                //resizedBitmap.recycle();


            }catch(Exception e){ // Catch the download exception
                e.printStackTrace();
            }
            return logo;
        }

        /*
            onPostExecute(Result result)
                Runs on the UI thread after doInBackground(Params...).
         */
        protected void onPostExecute(Bitmap result){
            imageView.setImageBitmap(result);
        }
    }

}
