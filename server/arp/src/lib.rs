use libarp::{client::ArpClient, interfaces::MacAddr};
use safer_ffi::prelude::*;
use std::{net::Ipv4Addr, time::Duration};

#[cfg(feature = "headers")]
pub fn generate_headers() -> ::std::io::Result<()> {
    ::safer_ffi::headers::builder()
        .to_file("arp.h")?
        .generate()
}

/// Consistently sends ARP requests for multiple IPs and waits
///
/// # Arguments
///
/// * `ips` - C-like array with C-strings containing human representation of IP
/// * `timeout_sec` - Timeout in seconds after which false will be returned
///
/// # Returns
///
/// `false` if running device has no IP allocated or no IPs responded
#[safer_ffi::ffi_export]
pub fn arp_ping_multiple<'a>(ips: c_slice::Mut<'a, char_p::Ref<'a>>, timeout_sec: u64) -> bool {
    let client = ArpClient::new();
    if client.is_err() {
        return false;
    }
    let mut client = client.unwrap();

    let ips: Vec<Ipv4Addr> = ips
        .iter()
        .filter_map(|ip| ip.to_str().parse().ok())
        .collect();

    for target_ip in ips.into_iter() {
        let mac: Result<MacAddr, _> = client.ip_to_mac(target_ip, Some(Duration::from_secs(timeout_sec)));
        if let Ok(mac) = mac {
            println!("found {:?} with mac {}", target_ip, mac);
            return true;
        } else {
            println!("ip {:?} hasn't responded in time", target_ip);
        }
    }

    false
}
