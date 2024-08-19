
use std::str::FromStr;
use std::sync::mpsc::channel;
use std::thread;
use std::{thread::sleep, time::Duration};
use std::net::{UdpSocket, SocketAddr};
use std::env;

use serialport::UsbPortInfo;
use toy_arms::external::{read, Process};
use inputbot::{KeySequence, KeybdKey::*, MouseButton::*};

// This program presses keys for the Arduino and fetches lights from the DDR clone

fn main(){

    println!("Usage: outfox-lights.exe [part of portname]");
    
    let (tx,rx) = channel::<u8>();
    let args: Vec<String> = env::args().collect();
    let mut args_iter = args.iter();
    args_iter.next();

    let portname = args_iter.next().unwrap_or(&String::new()).clone();


    thread::spawn(move ||{
        let keys = vec![DownKey, RightKey, LeftKey,UpKey , EnterKey, EscapeKey];


        let ports = serialport::available_ports().expect("No ports found!");

        println!(" -- Available ports: --");
        for port in &ports{
            println!("{}", port.port_name);
        }
        println!();

        // println!("{:?}", ports);
        let portname = ports.iter().find(|x| return x.port_name.contains(&portname)
        ).expect("No such port!!").port_name.clone();

        println!("Using port {portname}");

        let mut port = serialport::new(portname, 115200).timeout(Duration::from_millis(1000)).open().expect("Port open failed");
        port.write_data_terminal_ready(true);

        port.set_timeout(Duration::from_secs(999999999)).ok();
        let mut buf = vec![0; 1];
        loop{
            port.read_exact(&mut buf).unwrap();
            println!("Got {:?}", buf);
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
                tx.send(*byte);

                let mut val = byte & 0b0111111;

                if keys[val as usize] == LeftKey{

                }
                

                if (byte & 0b10000000 > 0) {
                    println!("press {:?}", keys[val as usize]);
                    keys[val as usize].press();
                }else{
                    println!("release {:?}", keys[val as usize]);
                    keys[val as usize].release();

                }
                


            }
        }
    });



    let of = Process::from_process_name("OutFox.exe").unwrap();
    let h = of.process_handle;
    let base = of.get_module_base("OutFox.exe").unwrap();
    let addr = base + 0x1BE37F0;


    let address = SocketAddr::from_str("127.0.0.1:6969").unwrap();
    let socket = UdpSocket::bind("127.0.0.1:5433").unwrap();

    let mut lights = vec![0; 4];

    loop{
        while let Ok(msg) = rx.try_recv(){
            let val = msg & 0b0111111;
            if val < 4{
                lights[val as usize] = (msg & 0b10000000 > 0) as u8;
            }
        }
        let mut vals: Vec<u8> = (0..6).map(|x| read(h, addr + x).unwrap()).collect();
        vals.extend(lights.iter());
        socket.send_to(&vals, address).unwrap();
        sleep(Duration::from_millis(20));

    }

}
