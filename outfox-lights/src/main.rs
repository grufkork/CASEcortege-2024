
use std::str::FromStr;
use std::thread;
use std::{thread::sleep, time::Duration};
use std::net::{UdpSocket, SocketAddr};

use toy_arms::external::{read, Process};
use inputbot::{KeySequence, KeybdKey::*, MouseButton::*};

// This program presses keys for the Arduino and fetches lights from the DDR clone

fn main(){
    thread::spawn(||{
        let keys = vec![UpKey, LeftKey, RightKey,DownKey , EnterKey, EscapeKey];


        let ports = serialport::available_ports().expect("No ports found!");
        let portname = ports.first().unwrap();
        let mut port = serialport::new(portname.port_name.clone(), 115200).timeout(Duration::from_millis(1000)).open().expect("Port open failed");
        port.write_data_terminal_ready(true);
        port.set_timeout(Duration::from_secs(999999999)).ok();
        let mut buf = vec![0; 1];
        loop{
            port.read_exact(&mut buf).unwrap();
            println!("Got {:?}", buf);
            for byte in &buf{

                let val = byte & 0b0111111;
                if byte & 0b10000000 > 0{
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

    loop{
        let vals: Vec<u8> = (0..6).map(|x| read(h, addr + x).unwrap()).collect();
        socket.send_to(&vals, address).unwrap();
        sleep(Duration::from_millis(20));

    }

}
