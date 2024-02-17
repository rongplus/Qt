//
//  MyCollectionViewCell.swift
//  CView
//
//  Created by Rong Huang on 9/26/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class MyCollectionViewCell: UICollectionViewCell
{
  
    @IBOutlet weak var imgHead: UIImageView!
    
    @IBOutlet weak var lbName: UILabel!
    override func awakeFromNib() {
        super.awakeFromNib()
        // Initialization code
        lbName.textColor = UIColor.white
        lbName.textAlignment = .center
        lbName.backgroundColor = UIColor(red: 0, green: 0, blue: 0, alpha: 0.2)
    
        //imgHead.frame = bounds
        imgHead.contentMode = .scaleAspectFit
     
        //imgHead.backgroundColor = UIColor(red: 0, green: 0, blue: 50, alpha: 0.2)
    
    }
}
