use owasm::oei;

#[no_mangle]
pub fn prepare() {
    oei::request_external_data(1, 1, "band-protocol".as_bytes());
}

#[no_mangle]
pub fn execute() {
    let data = oei::get_external_data(1, 0);
    oei::save_return_data(&data);
}