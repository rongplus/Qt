package com.sharkversion.rong.sharkview;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;



import android.support.design.widget.TabLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v4.view.PagerAdapter;
import android.view.View;
import  android.widget.TextView;
import android.view.ViewGroup;
import java.util.List;
import java.util.ArrayList;
import android.view.LayoutInflater;
import android.widget.ListView;
import android.widget.ArrayAdapter;
import  java.util.*;

import  java.net.HttpURLConnection;
import java.io.*;
import  java.net.*;
import android.util.Log;

import java.net.HttpURLConnection;
import android.net.ConnectivityManager;
import 	android.net.NetworkInfo;
import android.os.AsyncTask;
import org.json.JSONObject;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

import android.widget.Toast;

import android.content.Intent;

import 	android.widget.SimpleAdapter;


public class MainView extends AppCompatActivity {

    private ViewPager mViewPager;
    private View view1, view2, view3;//需要滑动的页卡
    private List<View> views;// Tab页面列表
    private ArrayList<String> titleList;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_view);

        TabLayout tab = (TabLayout) findViewById(R.id.switchWidget);
        mViewPager = (ViewPager) findViewById(R.id.mainWidget);

        tab.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener()
        {

            public void onTabSelected(TabLayout.Tab t){
                //viewPager.setCurrentItem(t.getPosition());
                mViewPager.setCurrentItem(t.getPosition());
            }
            public void onTabUnselected (TabLayout.Tab t){

            }
            public void onTabReselected (TabLayout.Tab t){

            }
        });


        initView();

        InitPage1();
        InitPage2();
        InitPage3();
        InitPage4();

        PagerAdapter pagerAdapter = new PagerAdapter() {


            public boolean isViewFromObject(View arg0, Object arg1) {
                return arg0 == arg1;
            }


            public int getCount() {
                return views.size();
            }


            public void destroyItem(ViewGroup container, int position,
                                    Object object) {
                container.removeView(views.get(position));
            }


            public int getItemPosition(Object object) {
                return super.getItemPosition(object);
            }


            public CharSequence getPageTitle(int position) {
                return titleList.get(position);
            }


            public Object instantiateItem(ViewGroup container, int position) {
                container.addView(views.get(position));
                return views.get(position);
            }

        };
        mViewPager.setAdapter(pagerAdapter);
    }


    private void initView() {
        LayoutInflater lf = getLayoutInflater().from(this);
        view1 = lf.inflate(R.layout.page1, null);
        view2 = lf.inflate(R.layout.page2, null);
        view3 = lf.inflate(R.layout.page3, null);

        views = new ArrayList<View>();// 将要分页显示的View装入数组中
        views.add(view1);
        views.add(view2);
        views.add(view3);



    }

    private  void InitPage1()
    {
        ListView weblist = (ListView) view1.findViewById(R.id.weblist);

        List<Map<String, Object>> data1 = new ArrayList< Map<String, Object> > ();
        Map<String, Object> item = new HashMap<String, Object>();
        item.put("itemTitle","item1");
        item.put("itemPhoto",R.drawable.call);

        item.put("itemSummary","item1");
        item.put("itemAuthor","item1");
        item.put("itemPublishtime","item1");

        data1.add(item);

        item = new HashMap<String, Object>();
        item.put("itemTitle","item2");
        item.put("itemPhoto", R.drawable.email);
        item.put("itemSummary","item2");
        item.put("itemAuthor","item2");
        item.put("itemPublishtime","item2");

        data1.add(item);


        //创建简单适配器SimpleAdapter
        SimpleAdapter simpleAdapter = new SimpleAdapter(this, data1, R.layout.weblistitem,
                new String[] {"itemTitle","itemPhoto", "itemSummary", "itemAuthor", "itemPublishtime"},
                new int[] {R.id.title, R.id.photograph, R.id.summary, R.id.author, R.id.publishtime});

        //加载SimpleAdapter到ListView中
        weblist.setAdapter(simpleAdapter);
    }

    private  void InitPage2() {

    }

    private  void InitPage3()
    {

    }

    private  void InitPage4()
    {

    }

}
