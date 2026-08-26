import { useState } from "react";
import "./LoginScreen.css";

export default function LoginScreen({ onLogin }: { onLogin: () => void }) {
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const handleLogin = (event: React.FormEvent) => { event.preventDefault(); if (password === "1234") { setError(""); setPassword(""); onLogin(); } else { setError("Incorrect password"); setPassword(""); } };
  return <div className="login-container"><div className="login-box"><h1>🔐 Password Manager</h1><form onSubmit={handleLogin}><input type="password" value={password} onChange={(event) => setPassword(event.target.value)} placeholder="Enter master password" className="login-input" autoFocus /><button type="submit" className="login-button">Unlock</button></form>{error && <p className="error-message">{error}</p>}</div></div>;
}