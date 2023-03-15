mod color;
mod i2c;

use self::{color::Color, i2c::RequestMessage};
use crate::relative_time::RelativeTime;
use anyhow::Result;
use chrono::{Local, Timelike};
use i2cdev::{core::I2CDevice, linux::LinuxI2CDevice};
use serde::Deserialize;
use std::{
    mem,
    ops::RangeInclusive,
    sync::atomic::{AtomicBool, Ordering},
};
use tokio::time::sleep;

pub(crate) static ANYONE_AT_HOME: AtomicBool = AtomicBool::new(false);

type Buffer = [u8; 32];

#[derive(Deserialize)]
#[serde(default)]
pub(crate) struct Config {
    bus: u8,
    address: u16,
    update_timeout: RelativeTime,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            bus: 1,
            address: 0x8,
            update_timeout: RelativeTime::Seconds(2),
        }
    }
}

pub(crate) struct Arduino {
    config: Config,
    connection: LinuxI2CDevice,
    day_colors: Vec<(RangeInclusive<u8>, Color)>,
}

impl Arduino {
    pub(crate) fn new(config: Config) -> Result<Self> {
        let day_colors = vec![
            (0..=5, Color::new(1, 0, 0, 0)),
            (6..=7, Color::new(150, 35, 2, 0)),
            (8..=21, Color::new(255, 80, 7, 255)),
            (21..=23, Color::new(60, 20, 1, 0)),
        ];

        let connection = LinuxI2CDevice::new(format!("/dev/i2c-{}", config.bus), config.address)?;

        Ok(Self {
            config,
            connection,
            day_colors,
        })
    }

    pub(crate) async fn run(mut self) {
        loop {
            let _ = self.update_color();

            sleep(self.config.update_timeout.into()).await;
        }
    }

    fn update_color(&mut self) -> Result<()> {
        let new_color = if ANYONE_AT_HOME.load(Ordering::Acquire) {
            self.color_for_current_hour()
        } else {
            Color::default()
        };

        log::info!("Setting color to {:?}", new_color);

        let mut buffer: Buffer = unsafe { mem::zeroed() };

        RequestMessage::set_color(new_color).copy(&mut buffer);

        let effective_buffer = &buffer[..buffer[0] as usize];
        self.connection.write(effective_buffer)?;

        Ok(())
    }

    fn color_for_current_hour(&self) -> Color {
        let current_hour = Local::now().hour() as u8;
        log::info!("Current hour = {}", current_hour);

        self.day_colors
            .iter()
            .find(|(hour_range, _)| hour_range.contains(&current_hour))
            .map(|(_, color)| *color)
            .unwrap_or_default()
    }
}
