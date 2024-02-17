//
//  DajiaTableViewCell.swift
//  dajia
//
//  Created by Rong Huang on 9/27/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class DajiaTableViewCell: UITableViewCell {

    //MARK
    @IBOutlet weak var imgHead1: UIImageView!
    @IBOutlet weak var imgHead2: UIImageView!
    @IBOutlet weak var lb1: UILabel!
    @IBOutlet weak var lb2: UILabel!
    
    //MARK
    override func awakeFromNib() {
       
       
   
        super.awakeFromNib()
        // Initialization code
        
        imgHead1.frame =  CGRect(x: 0, y: 0, width: 100, height: 80)
        imgHead2.frame =  CGRect(x: 0, y: 0, width: 100, height: 80)
        imgHead1.translatesAutoresizingMaskIntoConstraints = true
        //imgHead1.contentMode = .scaleAspectFit
        //imgHead1.layer.masksToBounds = YES;
        //imgHead1.backgroundColor = UIColor(red: 0, green: 0, blue: 50, alpha: 0.2)
        
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
