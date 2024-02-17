//
//  MyTableViewCell.swift
//  dajia2
//
//  Created by Rong Huang on 9/19/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class MyTableViewCell: UITableViewCell {

    //MARK: Properties
    
    @IBOutlet weak var img_p1: UIImageView!
    @IBOutlet weak var img_p2: UIImageView!
    @IBOutlet weak var txt1: UITextField!
    @IBOutlet weak var lbl1: UILabel!
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
    }

    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }

}
