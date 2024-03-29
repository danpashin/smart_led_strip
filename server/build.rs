use std::error::Error;
use vergen::EmitBuilder;

fn main() -> Result<(), Box<dyn Error>> {
    EmitBuilder::builder()
        .all_build()
        .all_cargo()
        .all_rustc()
        .fail_on_error()
        .emit()?;
    Ok(())
}
