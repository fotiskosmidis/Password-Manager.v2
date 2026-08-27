use std::ffi::{c_char, CStr, CString};

unsafe extern "C" {
    fn pm_create_key_file(password: *const c_char) -> i32;
    fn pm_key_file_exists() -> i32;
    fn pm_validate_password(password: *const c_char) -> i32;
    fn pm_load_data() -> i32;
    fn pm_save_data() -> i32;
    fn pm_add_email(email_address: *const c_char) -> i32;
    fn pm_remove_email(email_address: *const c_char) -> i32;
    fn pm_get_email_count() -> i32;
    fn pm_get_email_address(index: i32) -> *mut c_char;
    fn pm_add_account(email_address: *const c_char, company: *const c_char,
                      password: *const c_char, name: *const c_char) -> i32;
    fn pm_remove_account(email_address: *const c_char, account_id: i32) -> i32;
    fn pm_get_account_count(email_address: *const c_char) -> i32;
    fn pm_get_account_id(email_address: *const c_char, index: i32) -> i32;
    fn pm_get_account_company(email_address: *const c_char, account_id: i32) -> *mut c_char;
    fn pm_get_account_password(email_address: *const c_char, account_id: i32) -> *mut c_char;
    fn pm_get_account_name(email_address: *const c_char, account_id: i32) -> *mut c_char;
    fn pm_set_account_company(email_address: *const c_char, account_id: i32,
                              value: *const c_char) -> i32;
    fn pm_set_account_password(email_address: *const c_char, account_id: i32,
                               value: *const c_char) -> i32;
    fn pm_set_account_name(email_address: *const c_char, account_id: i32,
                           value: *const c_char) -> i32;
    fn pm_string_free(value: *mut c_char);
}

fn text(value: &str) -> Result<CString, String> {
    CString::new(value).map_err(|_| "Values cannot contain a null byte".to_string())
}

unsafe fn take_string(value: *mut c_char) -> Result<String, String> {
    if value.is_null() {
        return Err("C++ returned no value".to_string());
    }
    let result = CStr::from_ptr(value).to_string_lossy().into_owned();
    pm_string_free(value);
    Ok(result)
}

pub fn create_key_file(password: &str) -> Result<bool, String> {
    let password = text(password)?;
    let result = unsafe { pm_create_key_file(password.as_ptr()) };
    match result { 1 => Ok(true), 0 => Ok(false), _ => Err("Could not create key file".to_string()) }
}

pub fn key_file_exists() -> bool {
    unsafe { pm_key_file_exists() == 1 }
}

pub fn validate_password(password: &str) -> Result<bool, String> {
    let password = text(password)?;
    let result = unsafe { pm_validate_password(password.as_ptr()) };
    match result { 1 => Ok(true), 0 => Ok(false), _ => Err("Could not validate password".to_string()) }
}

pub fn load_data() -> Result<(), String> {
    if unsafe { pm_load_data() } == 0 { Ok(()) } else { Err("Could not load data".to_string()) }
}

pub fn save_data() -> Result<(), String> {
    if unsafe { pm_save_data() } == 0 { Ok(()) } else { Err("Could not save data".to_string()) }
}

pub fn email_addresses() -> Result<Vec<String>, String> {
    let count = unsafe { pm_get_email_count() };
    if count < 0 { return Err("Could not read emails".to_string()); }
    (0..count).map(|index| unsafe { take_string(pm_get_email_address(index)) }).collect()
}

pub fn add_email(email: &str) -> Result<(), String> {
    let email = text(email)?;
    if unsafe { pm_add_email(email.as_ptr()) } == 0 { Ok(()) } else { Err("Could not add email".to_string()) }
}

pub fn remove_email(email: &str) -> Result<(), String> {
    let email = text(email)?;
    if unsafe { pm_remove_email(email.as_ptr()) } == 0 { Ok(()) } else { Err("Could not remove email".to_string()) }
}

pub fn add_account(email: &str, company: &str, password: &str, name: &str) -> Result<i32, String> {
    let email = text(email)?; let company = text(company)?; let password = text(password)?; let name = text(name)?;
    let result = unsafe { pm_add_account(email.as_ptr(), company.as_ptr(), password.as_ptr(), name.as_ptr()) };
    if result >= 0 { Ok(result) } else { Err("Could not add account".to_string()) }
}

pub fn remove_account(email: &str, account_id: i32) -> Result<(), String> {
    let email = text(email)?;
    if unsafe { pm_remove_account(email.as_ptr(), account_id) } == 0 { Ok(()) } else { Err("Could not remove account".to_string()) }
}

pub fn accounts(email: &str) -> Result<Vec<(i32, String, String, String)>, String> {
    let email = text(email)?;
    let count = unsafe { pm_get_account_count(email.as_ptr()) };
    if count < 0 { return Err("Could not read accounts".to_string()); }
    (0..count).map(|index| unsafe {
        let id = pm_get_account_id(email.as_ptr(), index);
        if id < 0 { return Err("Could not read account ID".to_string()); }
        Ok((id, take_string(pm_get_account_company(email.as_ptr(), id))?,
            take_string(pm_get_account_name(email.as_ptr(), id))?,
            take_string(pm_get_account_password(email.as_ptr(), id))?))
    }).collect()
}

pub fn set_account(email: &str, account_id: i32, company: &str, password: &str, name: &str) -> Result<(), String> {
    let email = text(email)?; let company = text(company)?; let password = text(password)?; let name = text(name)?;
    let results = unsafe {
        [pm_set_account_company(email.as_ptr(), account_id, company.as_ptr()),
         pm_set_account_password(email.as_ptr(), account_id, password.as_ptr()),
         pm_set_account_name(email.as_ptr(), account_id, name.as_ptr())]
    };
    if results.iter().all(|result| *result == 0) { Ok(()) } else { Err("Could not update account".to_string()) }
}