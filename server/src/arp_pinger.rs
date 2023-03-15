use crate::relative_time::RelativeTime;
use anyhow::Result;
use libarp::{client::ArpClient, interfaces::Interface};
use serde::Deserialize;
use std::{
    convert::Infallible,
    net::Ipv4Addr,
    sync::atomic::{AtomicBool, Ordering},
};
use tokio::{task::JoinSet, time::sleep};

#[derive(Debug, Deserialize)]
#[serde(default)]
pub(crate) struct Config {
    interface: Option<String>,
    ips: Vec<Ipv4Addr>,
    generic_timeout: RelativeTime,
    ping_timeout: RelativeTime,
    ping_attempts_before_fail: u8,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            interface: None,
            ips: vec![],
            generic_timeout: RelativeTime::Seconds(8),
            ping_timeout: RelativeTime::Seconds(10),
            ping_attempts_before_fail: 10,
        }
    }
}

#[derive(Debug)]
pub(crate) struct ARPing {
    interface: Interface,
    config: Config,
}

impl ARPing {
    pub(crate) fn new(config: Config) -> Result<Self> {
        let iface = match &config.interface {
            Some(interface_name) => Interface::new_by_name(interface_name)?,
            None => Interface::new()?,
        };

        Ok(Self {
            interface: iface,
            config,
        })
    }

    pub(crate) async fn ping(&self, result: &AtomicBool) -> Result<Infallible> {
        let mut failed_count = 0;

        loop {
            let mut ping_tasks = JoinSet::new();
            for device in &self.config.ips {
                let device = *device;

                let mut client = ArpClient::new_with_iface(&self.interface)?;
                let timeout = Some(self.config.ping_timeout.into());
                ping_tasks.spawn(async move { (device, client.ip_to_mac(device, timeout).await) });
            }

            let mut anyone_respond = false;

            while let Some(task) = ping_tasks.join_next().await {
                let (ip, ping_result) = task?;

                anyone_respond |= ping_result.is_ok();
                log::info!("{} is ok: {}", ip, ping_result.is_ok());
            }

            log::info!("anyone_respond = {anyone_respond}");

            if anyone_respond {
                failed_count = 0;
                result.store(true, Ordering::Release);
            } else {
                failed_count += 1;

                if failed_count >= self.config.ping_attempts_before_fail {
                    failed_count = 0;
                    result.store(false, Ordering::Release);
                }
            }

            sleep(self.config.generic_timeout.into()).await;
        }
    }
}
