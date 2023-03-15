#[repr(C)]
#[derive(Debug, Default, Copy, Clone)]
pub(crate) struct Color {
    pub(crate) red: u8,
    pub(crate) green: u8,
    pub(crate) blue: u8,
    pub(crate) white: u8,
}

impl Color {
    pub(crate) fn new(red: u8, green: u8, blue: u8, white: u8) -> Self {
        Self {
            red,
            green,
            blue,
            white,
        }
    }

    pub(crate) fn copy(self, buffer: &mut [u8]) -> u8 {
        buffer[0] = self.red;
        buffer[1] = self.green;
        buffer[2] = self.blue;
        buffer[3] = self.white;

        4
    }
}
