#[derive(Copy, Clone)]
#[repr(u8)]
pub(crate) enum ResponseKind {
    Success = 3,
}

#[repr(C)]
pub(crate) struct ResponseMessage {
    pub(crate) length: u8,
    pub(crate) kind: ResponseKind,
    pub(crate) payload: [u8; 30],
}
