//
//  ViewController.swift
//  ControlLEDESP8266
//
//  Created by TuanVi Phan on 11/4/17.
//  Copyright © 2017 TuanVi Phan. All rights reserved.
//

import UIKit
import SocketIO

class ViewController: UIViewController {
    
    var socket: SocketIOClient!
    var isConnected = false
    var isConnectedFail: Bool = true
    var oneTime = true
    
    @IBAction func switchAction(_ sender: UISwitch) {
        if sender.isOn {
            socket.emit("client2server-control-led", with: [true])
        } else {
            socket.emit("client2server-control-led", with: [false])
        }
    }
    @IBAction func connectTapped(_ sender: UIButton) {
        if !isConnected {
            var nameServer = nameServerTextField.text!
            if (nameServerTextField.text! == "") {
                nameServer = "esp2led.herokuapp.com"
            }
            socket = SocketIOClient(socketURL: URL(string: "http://\(nameServer)")!, config: [.log(true), .compress])
            socket.connect()
            
            socket.on(clientEvent: SocketClientEvent.connect, callback: { (data, ack) in
                
                self.isConnectedFail = false
                
                self.connectButton.setTitle("Disconnect", for: UIControlState.normal)
                self.nameServerTextField.isEnabled = false
                self.switchDisplay.isEnabled = true
                self.isConnected = true
                
                self.listenFromServer()
            })
            
            socket.on(clientEvent: SocketClientEvent.error, callback: { (data, ack) in
                guard let message = data[0] as? String else { return }
                
                if (self.isConnectedFail && self.oneTime) {
                    self.alertMessage(message: message)
                    self.oneTime = false
                }
            })
            
            socket.on(clientEvent: SocketClientEvent.disconnect, callback: { (data, ack) in
                
                self.statusLedLabel.text = "Disconnected from SocketIO server"
            })
        } else {
            socket.disconnect()
            connectButton.setTitle("Connect", for: UIControlState.normal)
            nameServerTextField.isEnabled = true
            switchDisplay.isEnabled = false
            isConnected = false
        }
        
    }
    
    private func alertMessage(message: String) {
        let alertController = UIAlertController(title: "Alert", message: message, preferredStyle: UIAlertControllerStyle.alert)
        let alertAction = UIAlertAction(title: "OK", style: UIAlertActionStyle.default, handler: nil)
        alertController.addAction(alertAction)
        present(alertController, animated: true, completion: nil)
    }
    
    private func listenFromServer() {
        socket.on("server2client-led-display") { (data, ack) in
            guard let json = data[0] as? [String: Bool] else { return }
            if json["led"]! {
                self.ledView.backgroundColor = .red
                self.switchDisplay.isOn = true
                self.statusLedLabel.text = "LED is on"
                self.switchDisplay.isEnabled = true
            } else {
                self.ledView.backgroundColor = .lightGray
                self.switchDisplay.isOn = false
                self.statusLedLabel.text = "LED is off"
                self.switchDisplay.isEnabled = true
            }
        }
        
        socket.on("server2client-esp-disconnected") { (data, ack) in
            guard let message = data[0] as? String else { return }
            self.statusLedLabel.text = message;
            self.switchDisplay.isEnabled = false;
        }
    }
    
    @IBOutlet weak var nameServerTextField: UITextField!
    @IBOutlet weak var connectButton: UIButton!
    @IBOutlet weak var switchDisplay: UISwitch!
    @IBOutlet weak var statusLedLabel: UILabel!
    @IBOutlet weak var ledView: UIView!
        
    override func viewDidLoad() {
        super.viewDidLoad()
        
        ledView.layer.cornerRadius = ledView.frame.size.height/2
        ledView.backgroundColor = .lightGray
        switchDisplay.isEnabled = false
        connectButton.setTitle("Connect", for: UIControlState.normal)
        
    }
}

