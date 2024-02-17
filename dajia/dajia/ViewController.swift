//
//  ViewController.swift
//  dajia
//
//  Created by Rong Huang on 9/27/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var img1: UIImageView!
    var person_name :String?
    @IBOutlet weak var reText: UITextView!
    @IBOutlet weak var img2: UIImageView!
    @IBOutlet weak var profesion: UITextView!
    
    @IBOutlet weak var gene: UITextView!
    
    static var isRead = false

    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        if let person_name = person_name {
            print ("+++++++++++++++++++++++person_name+++++++++++++++++++++")
            print(person_name)
            
        }
        //img1.frame = CGRect(x: 0, y: 0, width: 100, height: 180)
        img1.contentMode = .scaleAspectFit
        
        //img1.backgroundColor = UIColor(red: 0, green: 0, blue: 50, alpha: 0.2)

        //img2.frame = CGRect(x: 0, y: 0, width: 100, height: 180)
        img2.contentMode = .scaleAspectFit
        
       // img2.backgroundColor = UIColor(red: 0, green: 0, blue: 50, alpha: 0.2)

        
        img1.image =  UIImage(named: "fisher.jpeg")
        img2.image =  UIImage(named: "cc_0_1")
        reText.text = "五年的大型房地产开发公司（万达、远洋）工作经历，\n 验房技术背景有保障、专业扎实；四年的房地产开发、工程验收、设计规划的培训经历，\n 更懂得如何在验房的同时帮助您更好地了解您的房子、使用您的房子，维护您的房子；行业最权威机构Carson Dunlop经过一年系统的理论与实践学习经历，验房专业知识更扎实、更全面，验房工作从一开就站在行业的最高起点、最高标准、最严要求。\n联系验房：于师傅\n电话：6479726691，微信号：6479726691更多我的介绍、房屋保养的文章请上公司的网站查看：www.taihe.ca"
        
        profesion.text = "所属黄页】园艺铲雪【更新时间】2017年9月23日【服务价格】[ 面议 ]【联系人】李师傅 Lee联系电话】647-859-6666【电子邮件】[ 发送邮件 ]【其他电话】416-821-9939区】多地区"
        
        gene.text = "SharkVesion技术服务有限公司，成立于2008年，我公司为各行各业提供专业的IT技术服务，经过3年多的努力与发展，已具一定的规模及实力，现拥有一支技术精湛的IT服务团队，以卓越的服务品质、专业安全的技术服务实力，为不同群体的用户提供更高更优质的IT服务。"
        
        read_file()
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    func read_file()
    {
        if( ViewController.isRead == false)
        {
            print ("read - a file")
            ViewController.isRead  = true
        }
        else
        {
            print("already")
        }
    }

}

