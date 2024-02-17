//
//  MyRatingControl.swift
//  dajia2
//
//  Created by Rong Huang on 9/19/17.
//  Copyright © 2017 Rong Huang. All rights reserved.
//

import UIKit

class MyRatingControl: UIStackView {

    /*
    // Only override draw() if you perform custom drawing.
    // An empty implementation adversely affects performance during animation.
    override func draw(_ rect: CGRect) {
        // Drawing code
    }
    */
    //MARK: Properties 
    @IBInspectable var starCount: Int = 5
    //MARK: init
    override init(frame: CGRect) {
       super.init(frame: frame)
        setupButtons()
        setupButtons()
    }
    
    required init(coder: NSCoder) {
        super.init(coder: coder)
        setupButtons()
        setupButtons()
    }

    //MARK: Private Methods
    private func setupButtons() {
        // Create the button
        let button = UIButton()
        button.backgroundColor = UIColor.red
        // Add constraints
        button.translatesAutoresizingMaskIntoConstraints = false
        button.heightAnchor.constraint(equalToConstant: 44.0).isActive = true
        button.widthAnchor.constraint(equalToConstant: 44.0).isActive = true
        
        // Add the button to the stack
        addArrangedSubview(button)
    }
}
