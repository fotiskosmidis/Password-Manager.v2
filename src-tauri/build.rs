fn main() {
    let manifest_dir = std::env::var("CARGO_MANIFEST_DIR").unwrap();
    let build_dir = std::path::Path::new(&manifest_dir).join("../build");

    println!("cargo:rerun-if-changed={}", build_dir.display());

    #[cfg(target_os = "windows")]
    {
        let release_dir = build_dir.join("Release");
        println!("cargo:rustc-link-search=native={}", release_dir.display());
        println!("cargo:rustc-link-search=native={}", build_dir.display());
        println!("cargo:rustc-link-lib=dylib=password_manager_backend");
        println!("cargo:rustc-link-arg=/DELAYLOAD:password_manager_backend.dll");
        println!("cargo:rustc-link-lib=dylib=delayimp");
    }

    #[cfg(not(target_os = "windows"))]
    {
        println!("cargo:rustc-link-search=native={}", build_dir.display());
        println!("cargo:rustc-link-lib=dylib=password_manager_backend");
        println!("cargo:rustc-link-arg=-Wl,-rpath,$ORIGIN");
        println!("cargo:rustc-link-arg=-Wl,-rpath,$ORIGIN/../../../build");
    }

    tauri_build::build()
}
