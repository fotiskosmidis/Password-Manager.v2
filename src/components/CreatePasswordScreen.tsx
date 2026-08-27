import { useState } from "react";
import "./LoginScreen.css";
import { createKeyFile } from "../api";

export default function CreatePasswordScreen({ onCreated }: { onCreated: () => void }) {
  const [password, setPassword] = useState("");
  const [confirmation, setConfirmation] = useState("");
  const [error, setError] = useState("");

  const handleSubmit = async (event: React.FormEvent) => {
    event.preventDefault();
    if (password.length === 0) {
      setError("Enter a password");
      return;
    }
    if (password !== confirmation) {
      setError("Passwords do not match");
      return;
    }
    try {
      if (await createKeyFile(password)) {
        setError("");
        onCreated();
      } else {
        setError("Could not create password file");
      }
    } catch (error) {
      setError(String(error));
    }
  };

  return <div className="login-container"><div className="login-box"><h1>🔐 Password Manager</h1><form onSubmit={handleSubmit}><input type="password" value={password} onChange={(event) => setPassword(event.target.value)} placeholder="Create master password" className="login-input" autoFocus /><input type="password" value={confirmation} onChange={(event) => setConfirmation(event.target.value)} placeholder="Confirm master password" className="login-input" /><button type="submit" className="login-button">Create Password</button></form>{error && <p className="error-message">{error}</p>}</div></div>;
}