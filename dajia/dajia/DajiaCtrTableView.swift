//
//  DajiaCtrTableView.swift
//  dajia
//
//  Created by Rong Huang on 9/27/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit
import os.log

class DajiaCtrTableView: UITableViewController {

    var group :String?
    
    @IBOutlet weak var my_title: UINavigationItem!
    
    
    
    let courses = [
        ["name":"Tom:水管师傅","pic":"cc_0_0.png","text":"提供：荣","recommand":"me","pic_rec":"cc_0_1.png","text_rec":"normal"],
        ["name":"Machel：电工师傅","pic":"cc_0_1.png","text":"提供：荣","recommand":"me","pic_rec":"cc_1_0.png","text_rec":"normal"],
        ["name":"Steve：暖气师傅","pic":"cc_0_2.png","text":"提供：荣","recommand":"me","pic_rec":"cc_1_2.png","text_rec":"normal"],
        ["name":"Joone：园林设计","pic":"cc_1_0.png","text":"提供：荣","recommand":"me","pic_rec":"cc_2_0.png","text_rec":"normal"],
        ["name":"Cobel：草地维护","pic":"cc_1_1.png","text":"推荐：fisher","recommand":"me","pic_rec":"cc_1_2.png","text_rec":"normal"],
        ["name":"Loly：专修游泳池","pic":"cc_0_1.png","text":"王教练：运动教练","recommand":"推荐：fisher","pic_rec":"cc_1_1.png","text_rec":"normal"]
    ]

    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem()
        // Set up views if editing an existing Meal.
        if let group = group {
            print ("++++++++++++++++++++++++++++++++++++++++++++")
            print(group)
            my_title.title = group
            
            if let path = Bundle.main.path(forResource:"g1", ofType: "json") {
                do {
                    let data = try Data(contentsOf: URL(fileURLWithPath: path), options: .alwaysMapped)
                        if let json = try? JSON(data: data) as JSON
                        {
                            print("--------------")
                            print(json[0]["name"])
                        }
                    }
                catch let error {
                    print(error.localizedDescription)
                }
            } else {
                print("Invalid filename/path.")
            }
            
            
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return courses.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "DajiaCell", for: indexPath)as! DajiaTableViewCell


        // Configure the cell...
        cell.lb1.text =  courses[indexPath.item]["name"]
        cell.imgHead1.image = UIImage(named: courses[indexPath.item]["pic"]!)
        cell.lb2.text =  courses[indexPath.item]["text"]
        cell.imgHead2.image = UIImage(named: courses[indexPath.item]["pic_rec"]!)
        //cell.imgHead1.contentMode = .scaleAspectFit
        return cell
    }
    

    /*
    // Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

    @IBAction func goBack(_ sender: UIBarButtonItem) {
        //dismiss(animated: true, completion: nil)
        
        let isPresentingInAddMealMode = presentingViewController is UINavigationController
        
        if isPresentingInAddMealMode {
            dismiss(animated: true, completion: nil)
        }
        else if let owningNavigationController = navigationController{
            owningNavigationController.popViewController(animated: true)
        }
        else {
            fatalError("The MealViewController is not inside a navigation controller.")
        }

    }
    
    
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        
        super.prepare(for: segue, sender: sender)
        
        switch(segue.identifier ?? "") {
            
        case "AddItem":
            os_log("Adding a new meal.", log: OSLog.default, type: .debug)
            
        case "ShowDetail":
            guard let ctr = segue.destination as? ViewController else {
                fatalError("Unexpected destination: \(segue.destination)")
            }
            
            guard let selectedCell = sender as? DajiaTableViewCell else {
                fatalError("Unexpected sender: \(sender)")
            }
            
            guard let indexPath = tableView.indexPath(for: selectedCell) else {
                fatalError("The selected cell is not being displayed by the table")
            }

            
            let person_name = courses[indexPath.item]["name"]
            ctr.person_name = person_name
            
        default:
            fatalError("Unexpected Segue Identifier; \(segue.identifier)")
        }
    }
    
    func read_json()
    {
        print("read file")
        if let path = Bundle.main.path(forResource:"a", ofType: "json")
        {
            // print(path)
            do
            {
                let jsonStr = try String(contentsOfFile: path)
                print("file content\n")
                
                
                let lines = jsonStr.components(separatedBy: "\n")
                for line in lines {
                    let words = line.components(separatedBy: " ")
                    print("------\(words[0]) ")
                }
                
                print(jsonStr)
                
                /*let data = try Data(contentsOf: URL(fileURLWithPath: path), options: .alwaysMapped)
                 if let json = try? JSON(data: data) as JSON
                 {
                 print("--------------")
                 print(json[0]["name"])
                 }*/
                
                if let jsonData = jsonStr.data(using: String.Encoding.utf8, allowLossyConversion: false) {
                    //.........
                    if let userArray = try? JSONSerialization.jsonObject(with: jsonData,
                                                                         options: .allowFragments) as? [[String: AnyObject]],
                        let phones = userArray?[0]["phones"] as? [[String: AnyObject]],
                        let number = phones[0]["number"] as? String
                    {
                        // 找到电话号码
                        print("第一个联系人的第一个电话号码：",number)
                    }
                }
                
                //----------------
            }
            catch let error
            {
                print(error.localizedDescription)
            }
        }
        else
        {
            print("Invalid filename/path.")
        }
        
    }
}
