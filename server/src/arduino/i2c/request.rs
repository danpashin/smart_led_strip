use crate::arduino::color::Color;

#[repr(u8)]
#[non_exhaustive]
pub(crate) enum RequestKind {
    SetColor(Color) = 4,
}

impl RequestKind {
    pub(crate) fn discriminant(&self) -> u8 {
        unsafe { *<*const _>::from(self).cast::<u8>() }
    }
}

#[repr(C)]
pub(crate) struct RequestMessage {
    pub(crate) kind: RequestKind,
}

impl RequestMessage {
    pub(crate) fn set_color(color: Color) -> Self {
        Self {
            kind: RequestKind::SetColor(color),
        }
    }

    pub(crate) fn copy(&self, buffer: &mut [u8; 32]) {
        buffer[0] = 2;
        buffer[1] = self.kind.discriminant();

        let RequestKind::SetColor(color) = &self.kind;
        buffer[0] += color.copy(&mut buffer[2..]);
    }
}
