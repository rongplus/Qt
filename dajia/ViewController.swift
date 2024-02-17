//
//  ViewController.swift
//  TestNet
//
//  Created by Rong Huang on 9/28/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var im1: UIImageView!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        /*
        // 方法一: 同步加载网络图片
        let url = URL(string: "http://i.imgur.com/w5rkSIj.jpg")
        // 从url上获取内容
        // 获取内容结束才进行下一步
        let data = try? Data(contentsOf: url!)
        
        if let imageData = data {
            let image = UIImage(data: data!)
            im1.image = image
        }
        */
        
        let urlString = "https://api.whitehouse.gov/v1/petitions.json?limit=100"
        
        if let url = URL(string: urlString) {
            if let data = try? String(contentsOf: url) {
                let json = JSON(parseJSON: data)
                
                if json["metadata"]["responseInfo"]["status"].intValue == 200 {
                    // we're OK to parse!
                    print("get")
                parse(json: json)
                }
            }
        }
        
        abc()
        ppp()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    func parse(json: JSON) {
        for result in json["results"].arrayValue {
            let title = result["title"].stringValue
            let body = result["body"].stringValue
            let sigs = result["signatureCount"].stringValue
            let obj = ["title": title, "body": body, "sigs": sigs]
            print(obj)
            //petitions.append(obj)
        }
         loadDataSource(isRefresh: false)
        //tableView.reloadData()
    }
    
    
    func loadDataSource(isRefresh : Bool) {
        print("loadDataSource \(isRefresh)")
 
        let jsonStr = "[{\"name\": \"hangge\", \"age\": 100, \"phones\": [{\"name\": \"公司\",\"number\": \"123456\"}, {\"name\": \"家庭\",\"number\": \"001\"}]}, {\"name\": \"big boss\",\"age\": 1,\"phones\": [{ \"name\": \"公司\",\"number\": \"111111\"}]}]"
        
        if let jsonData = jsonStr.data(using: String.Encoding.utf8, allowLossyConversion: false) {
            //.........
            if let userArray = try? JSONSerialization.jsonObject(with: jsonData,
                                                                 options: .allowFragments) as? [[String: AnyObject]],
                let phones = userArray?[0]["phones"] as? [[String: AnyObject]],
                let number = phones[0]["number"] as? String {
                // 找到电话号码
                print("第一个联系人的第一个电话号码：",number)
            }
            
            
        }
       
    
    }
    
    func abc()
    {
        //创建URL对象
        let url = URL(string:"http://192.168.4.227/a.json")
        //创建请求对象
        let request = URLRequest(url: url!)
        
        let dataTask = URLSession.shared.dataTask(with: request,
                  completionHandler:
            {(data, response, error) -> Void in
                  if error != nil{
                        print("err")
                  }
                  else
                  {
                    print(data)
                    
                    
                    if let json = try? JSON(data: data!) as JSON
                    {
                        if let number = json[0]["phones"][0]["number"].string {
                            // 找到电话号码
                            print("第一个联系人的第一个电话号码：",number)
                        }
                    }
                    
                   
                    
                    
                }
        }) as URLSessionTask
        
        //使用resume方法启动任务
        dataTask.resume()
    }
    
    func ppp()
    {
        
        
       
        
        //创建URL对象
        let url = URL(string: "http://i.imgur.com/w5rkSIj.jpg")

        //创建请求对象
        let request = URLRequest(url: url!)
        
        let dataTask = URLSession.shared.dataTask(with: request,
                                                  completionHandler:
            {(data, response, error) -> Void in
                if error != nil{
                    print("pic err")
                }
                else
                {
                    print("pic ok")
                    
                    
                    if data != nil {
                        let image = UIImage(data: data!)
                        self.im1.image = image
                    }

                    
                    
                    
                    
                }
        }) as URLSessionTask
        
        //使用resume方法启动任务
        dataTask.resume()
    }

}

