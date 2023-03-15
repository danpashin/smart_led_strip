use crate::{arduino, arp_pinger};
use log::LevelFilter;
use serde::Deserialize;

#[derive(Deserialize)]
#[serde(default)]
pub(crate) struct Config {
    pub(crate) log_level: LevelFilter,
    pub(crate) arduino: arduino::Config,
    pub(crate) pinger: arp_pinger::Config,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            log_level: LevelFilter::Warn,
            arduino: arduino::Config::default(),
            pinger: arp_pinger::Config::default(),
        }
    }
}

impl Config {
    pub(crate) fn setup_logger(&self) {
        simple_logger::SimpleLogger::new()
            .with_level(self.log_level)
            .init()
            .unwrap();
    }
}
