fn main() {
    println!("cargo:rustc-link-search=native=../build");
    println!("cargo:rustc-link-lib=dylib=password_manager_backend");
    println!("cargo:rustc-link-arg=-Wl,-rpath,$ORIGIN/../../../build");
    tauri_build::build()
}
