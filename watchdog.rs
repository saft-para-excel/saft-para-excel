use std::fs;
use std::thread::sleep;
use std::time::Duration;
use std::collections::HashMap;
use std::path::Path;
use chrono::prelude::*;

fn main() {
    let mut foo = 1;
    let mut foo2 = 1;

    let d = stdio::read_ini();
    let versions = stdio::read_version();
    let sleep_seconds = d.get("sleep_seconds").unwrap();
    let network_down_seconds = d.get("network_down_seconds").unwrap();
    
    println!("WatchDog {:>6}", versions.get("watch_dog").unwrap());
    
    let d = stdio::get_paths();
    let spool_dir = d.get("spool_dir").unwrap();
    stdio::mkdir(spool_dir);
    
    println!("Refresh: {} seconds", sleep_seconds);
    
    while foo == foo2 {
        let d = stdio::read_ini();
        let sleep_seconds = d.get("sleep_seconds").unwrap();
        let network_down_seconds = d.get("network_down_seconds").unwrap();
        
        let (response, d) = get_mail(spool_dir, false);
        if response {
            let working_dir = stdio::generate_hash_from_datetime(6);
            fs::rename(format!("{}/LOCK", spool_dir), format!("{}/{}", spool_dir, working_dir)).unwrap();
            d.insert("working_dir".to_string(), working_dir.clone());
            let file_size = fs::metadata(format!("{}/{}", spool_dir, d.get("rec_file").unwrap())).unwrap().len();
            d.insert("file_size".to_string(), file_size.to_string());
            d.insert("rec_date".to_string(), Utc::now().to_string());
            stdio::save_json(format!("{}/data.json", spool_dir), &d);
            log_crud::log_emails(d.get("to_mails").unwrap());
            log_crud::start(&d);
            
            println!("{}", "-".repeat(40));
            println!("       DATE: {}", Utc::now().format("%d-%m-%Y %H:%M"));
            println!("WORKING DIR: {}", d.get("working_dir").unwrap());
            println!("   FILENAME: {}", d.get("rec_name").unwrap());
            println!("     E_MAIL: {}", d.get("to_mails").unwrap());
            
            if file_size / 1024 > 999 {
                println!("  FILE SIZE: {} MB", file_size / 1024 / 1024);
            } else {
                println!("  FILE SIZE: {} KB", file_size / 1024);
            }
        } else {
            if *sleep_seconds == 0 {
                println!("\nExit by STOP command");
                foo = 2;
            } else {
                sleep(Duration::from_secs(*sleep_seconds));
            }
        }
    }
}

