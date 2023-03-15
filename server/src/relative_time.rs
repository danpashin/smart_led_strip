#![allow(unused)]

use chrono::{Duration, NaiveDateTime, Utc};
use serde::{de::Visitor, Deserialize, Deserializer};
use std::fmt::Formatter;
use std::num::ParseIntError;

pub(crate) enum ParseError {
    Empty,
    InvalidTimeUnit(String),
    ParseInt(ParseIntError),
}

#[derive(Copy, Clone, Debug)]
pub(crate) enum RelativeTime {
    Seconds(i64),
    Minutes(i64),
    Hours(i64),
    Days(i64),
    Months(i64),
    Years(i64),
}

impl RelativeTime {
    pub(crate) fn from_now_utc(self) -> NaiveDateTime {
        let datetime = Utc::now() + Duration::from(self);
        datetime.naive_utc()
    }
}

impl From<RelativeTime> for Duration {
    fn from(value: RelativeTime) -> Self {
        match value {
            RelativeTime::Seconds(value) => Duration::seconds(value),
            RelativeTime::Minutes(value) => Duration::minutes(value),
            RelativeTime::Hours(value) => Duration::hours(value),
            RelativeTime::Days(value) => Duration::days(value),
            RelativeTime::Months(value) => Duration::days(value * 30),
            RelativeTime::Years(value) => Duration::days(value * 365),
        }
    }
}

impl From<RelativeTime> for std::time::Duration {
    fn from(value: RelativeTime) -> Self {
        std::time::Duration::from_secs(Duration::from(value).num_seconds() as u64)
    }
}

impl<'a> TryFrom<&'a str> for RelativeTime {
    type Error = ParseError;

    fn try_from(value: &'a str) -> Result<Self, Self::Error> {
        let input_len = value.len();
        if input_len < 2 {
            return Err(ParseError::Empty);
        }

        let (time, unit) = value.split_at(input_len - 1);
        let time = time.parse::<i64>().map_err(ParseError::ParseInt)?;

        match unit {
            "s" => Ok(Self::Seconds(time)),
            "m" => Ok(Self::Minutes(time)),
            "h" => Ok(Self::Hours(time)),
            "d" => Ok(Self::Days(time)),
            "M" => Ok(Self::Months(time)),
            "y" => Ok(Self::Years(time)),
            _ => Err(ParseError::InvalidTimeUnit(unit.to_string())),
        }
    }
}

struct RelativeTimeVisitor;

impl<'de> Visitor<'de> for RelativeTimeVisitor {
    type Value = RelativeTime;

    fn expecting(&self, formatter: &mut Formatter<'_>) -> std::fmt::Result {
        formatter.write_str("a string, containing integer value and char time postfix")
    }

    fn visit_str<E>(self, v: &str) -> Result<Self::Value, E>
    where
        E: serde::de::Error,
    {
        RelativeTime::try_from(v)
            .map_err(|_| serde::de::Error::invalid_value(serde::de::Unexpected::Str(v), &self))
    }
}

impl<'de> Deserialize<'de> for RelativeTime {
    fn deserialize<D>(deserializer: D) -> Result<Self, D::Error>
    where
        D: Deserializer<'de>,
    {
        deserializer.deserialize_str(RelativeTimeVisitor)
    }
}
