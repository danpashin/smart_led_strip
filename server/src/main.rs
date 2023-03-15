#![deny(clippy::pedantic, rust_2018_idioms)]
#![allow(
    clippy::cast_possible_truncation,
    clippy::cast_sign_loss,
    clippy::wrong_self_convention
)]

mod arduino;
mod arp_pinger;
mod config;
mod relative_time;

use self::{
    arduino::{Arduino, ANYONE_AT_HOME},
    arp_pinger::ARPing,
    config::Config,
};
use anyhow::Result;
use clap::Parser;
use std::{fs::File, path::PathBuf};

#[derive(Parser)]
#[clap(about, version, long_version = long_version_message())]
struct CLIOptions {
    #[clap(long, short, value_parser)]
    config: PathBuf,
}

impl CLIOptions {
    fn read_config(&self) -> Result<Config> {
        let config_file = File::open(&self.config)?;
        Ok(serde_yaml::from_reader(config_file)?)
    }
}

const fn long_version_message() -> &'static str {
    concat!(
        env!("CARGO_PKG_VERSION"),
        "\nDebug: ",
        env!("VERGEN_CARGO_DEBUG"),
        "\nEnabled feautures: ",
        env!("VERGEN_CARGO_FEATURES"),
        "\nBuild on ",
        env!("VERGEN_BUILD_TIMESTAMP"),
        "\nWith rustc ",
        env!("VERGEN_RUSTC_SEMVER"),
        " for ",
        env!("VERGEN_CARGO_TARGET_TRIPLE"),
    )
}

#[tokio::main]
async fn main() -> Result<()> {
    let options: CLIOptions = CLIOptions::parse();
    let config = options.read_config()?;
    config.setup_logger();

    let pinger = ARPing::new(config.pinger)?;
    tokio::spawn(async move {
        loop {
            let _ = pinger.ping(&ANYONE_AT_HOME).await;
        }
    });

    let arduino = Arduino::new(config.arduino)?;
    arduino.run().await;

    Ok(())
}
