// Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
mod cpp_bridge;

#[cfg(target_os = "windows")]
fn load_native_backend() -> Result<(), String> {
    use std::ffi::OsStr;
    use std::os::windows::ffi::OsStrExt;
    use std::path::PathBuf;

    unsafe extern "system" {
        fn LoadLibraryW(name: *const u16) -> *mut std::ffi::c_void;
    }

    let mut candidates = Vec::new();
    if let Ok(executable) = std::env::current_exe() {
        if let Some(directory) = executable.parent() {
            candidates.push(directory.join("password_manager_backend.dll"));
            candidates.push(directory.join("resources/password_manager_backend.dll"));
        }
    }
    candidates.push(PathBuf::from("build/Release/password_manager_backend.dll"));

    for candidate in candidates {
        let wide: Vec<u16> = OsStr::new(&candidate).encode_wide().chain(Some(0)).collect();
        if unsafe { LoadLibraryW(wide.as_ptr()) }.is_null() == false {
            return Ok(());
        }
    }
    Err("Could not load password_manager_backend.dll".to_string())
}

#[cfg(not(target_os = "windows"))]
fn load_native_backend() -> Result<(), String> { Ok(()) }

#[tauri::command]
fn greet(name: &str) -> String {
    format!("Hello, {}! You've been greeted from Rust!", name)
}

#[tauri::command]
fn create_key_file(password: &str) -> Result<bool, String> {
    let created = cpp_bridge::create_key_file(password)?;
    if created {
        if !cpp_bridge::validate_password(password)? {
            return Err("Could not initialize password".to_string());
        }
        cpp_bridge::save_data()?;
    }
    Ok(created)
}

#[tauri::command]
fn key_file_exists() -> bool { cpp_bridge::key_file_exists() }

#[tauri::command]
fn validate_password(password: &str) -> Result<bool, String> { cpp_bridge::validate_password(password) }

#[tauri::command]
fn load_data() -> Result<(), String> { cpp_bridge::load_data() }

#[tauri::command]
fn save_data() -> Result<(), String> { cpp_bridge::save_data() }

#[tauri::command]
fn get_emails() -> Result<Vec<String>, String> { cpp_bridge::email_addresses() }

#[tauri::command]
fn add_email(email: &str) -> Result<(), String> {
    cpp_bridge::add_email(email)?;
    cpp_bridge::save_data()
}

#[tauri::command]
fn remove_email(email: &str) -> Result<(), String> {
    cpp_bridge::remove_email(email)?;
    cpp_bridge::save_data()
}

#[tauri::command]
fn add_account(email: &str, company: &str, password: &str, name: &str) -> Result<i32, String> {
    let id = cpp_bridge::add_account(email, company, password, name)?;
    cpp_bridge::save_data()?;
    Ok(id)
}

#[tauri::command]
fn get_accounts(email: &str) -> Result<Vec<(i32, String, String, String)>, String> { cpp_bridge::accounts(email) }

#[tauri::command]
fn remove_account(email: &str, account_id: i32) -> Result<(), String> {
    cpp_bridge::remove_account(email, account_id)?;
    cpp_bridge::save_data()
}

#[tauri::command]
fn update_account(email: &str, account_id: i32, company: &str, password: &str, name: &str) -> Result<(), String> {
    cpp_bridge::set_account(email, account_id, company, password, name)?;
    cpp_bridge::save_data()
}

#[cfg_attr(mobile, tauri::mobile_entry_point)]
pub fn run() {
    load_native_backend().expect("failed to load native password manager backend");
    tauri::Builder::default()
        .plugin(tauri_plugin_opener::init())
        .invoke_handler(tauri::generate_handler![
            greet, create_key_file, key_file_exists, validate_password, load_data, save_data,
            get_emails, add_email, remove_email, add_account, get_accounts,
            remove_account, update_account
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
