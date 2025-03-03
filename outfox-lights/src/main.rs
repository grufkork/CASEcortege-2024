use std::str::FromStr;
use std::sync::mpsc::channel;
use std::sync::mpsc::{Sender, Receiver};
use std::thread;
use std::{thread::sleep, time::Duration};
use std::net::{UdpSocket, SocketAddr};
use std::env;

use toy_arms::external::{read, Process};
use inputbot::KeybdKey::*;

// This program presses keys for the Arduino and fetches lights from the DDR clone

fn main(){

    println!("Usage: outfox-lights.exe [part of portname]");
    
    let (tx,rx) = channel::<u8>();
    let args: Vec<String> = env::args().collect();
    let mut args_iter = args.iter();
    args_iter.next();

    let portname = args_iter.next().unwrap_or(&String::new()).clone();


    thread::spawn(move ||{
        
        let ports = serialport::available_ports().expect("No ports found!");

        println!(" -- Available ports: --");
        for port in &ports{
            println!("{}", port.port_name);
        }
        println!();

        loop{
            let err = dancepad_routine(&portname, tx.clone());
            println!("Input routine error: {err}");
            thread::sleep(Duration::from_secs(3));
        }

    });


    loop{
        let err = outfox_routine(&rx);

        println!("Outfox routine error: {err}");
        thread::sleep(Duration::from_secs(3));
    }

    

}

fn outfox_routine(rx: &Receiver<u8>) -> String{
    let Ok(of) = Process::from_process_name("OutFox.exe") else{
        return "Outfox not found".to_string();
    };
    let h = of.process_handle;
    let Ok(base) = of.get_module_base("OutFox.exe")else{
        return "Outfox base not found".to_string();
    };
    let addr = base + 0x1BE37F0;


    let address = SocketAddr::from_str("127.0.0.1:6969").unwrap();
    let socket = UdpSocket::bind("127.0.0.1:5433").unwrap();

    let mut lights = [0; 4];

    loop{
        while let Ok(msg) = rx.try_recv(){
            let val = msg & 0b0111111;
            if val < 4{
                lights[val as usize] = (msg & 0b10000000 > 0) as u8;
            }
        }
        if let Ok(mut vals) = (0..6).map(|x| read(h, addr + x)).collect::<Result<Vec<u8>, _>>(){
            vals.extend(lights.iter());
            socket.send_to(&vals, address).unwrap();
        }else{
            return "Outfox read error".to_string();
        }
        sleep(Duration::from_millis(20));

    }
}

fn dancepad_routine(portname: &str, tx: Sender<u8>) -> String{


    let keys = [DownKey, RightKey, LeftKey,UpKey , EnterKey, EscapeKey];
    let Ok(ports) = serialport::available_ports() else{
        return "No ports found".to_string();
    };

    let Some(port) = ports.iter().find(|x| x.port_name.contains(portname)) else {
        return "Port not found".to_string();

    };
    let portname = port.port_name.clone();

    println!("Using port {portname}");

    let Ok(mut port) = serialport::new(portname, 115200).timeout(Duration::from_millis(1000)).open() else{
        return "Failed to open port".to_string();
    };
    if port.write_data_terminal_ready(true).is_err(){
        return "Failed to set port ready".to_string();
    }

    port.set_timeout(Duration::from_secs(9999)).ok();
    let mut buf = vec![0; 1];
    loop{
        if let Err(e) = port.read_exact(&mut buf){
            return format!("Failed to read: {e}");
        };
        println!("Read: {:?}", buf);
        for byte in &buf{
            // let mut byte = *byte;
            // if keys[(byte & 0b0111111) as usize] == LeftKey{
            //     if byte & 0b10000000 > 0{
            //         byte -= 0b10000000;
            //     }else{
            //         byte += 0b10000000;
            //     }
            //
            //
            // }
            if tx.send(*byte).is_err(){
                return "Failed to send".to_string();
            }

            let val = byte & 0b0111111;
            if val >= 6{
                continue;
            }



            if byte & 0b10000000 > 0 {
                print!("↓ {:?}", keys[val as usize]);
                keys[val as usize].press();
            }else{
                print!("↑ {:?}", keys[val as usize]);
                keys[val as usize].release();

            }

            println!("  {:0>3}", byte);


        }
    }
}

// !cargo r --release
