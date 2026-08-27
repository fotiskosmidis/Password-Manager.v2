import { useEffect, useState } from "react";
import "./MainWindow.css";
import EmailDetailsWindow from "./EmailDetailsWindow";
import type { EmailEntry } from "../types";
import { addEmail, getEmailEntries, loadData, removeEmail, saveData } from "../api";
export default function MainWindow() {
  const [emails, setEmails] = useState<EmailEntry[]>([]);
  useEffect(() => { loadData().then(getEmailEntries).then(setEmails).catch((error) => window.alert(String(error))); }, []);
  const [showForm, setShowForm] = useState(false); const [newEmail, setNewEmail] = useState(""); const [selectedEmail, setSelectedEmail] = useState<EmailEntry | null>(null);
  const handleAddEmail = async (e: React.FormEvent) => { e.preventDefault(); if (newEmail) { try { await addEmail(newEmail); await saveData(); setEmails([...emails, { id: Date.now(), email: newEmail, accounts: [] }]); setNewEmail(""); setShowForm(false); } catch (error) { window.alert(String(error)); } } };
  if (selectedEmail) return <EmailDetailsWindow email={selectedEmail} onBack={() => setSelectedEmail(null)} onUpdateEmail={(updated) => { setEmails(emails.map((entry) => entry.id === updated.id ? updated : entry)); setSelectedEmail(updated); }} />;
  return <div className="main-container"><div className="header"><h1>🔐 Password Manager</h1><button className="add-button" onClick={() => setShowForm(!showForm)}>{showForm ? "Cancel" : "+ Add Email"}</button></div>{showForm && <div className="add-form-container"><form onSubmit={handleAddEmail} className="add-form"><input type="email" value={newEmail} onChange={(e) => setNewEmail(e.target.value)} placeholder="Email address" required autoFocus /><button type="submit" className="save-button">Save</button></form></div>}<div className="emails-container">{emails.length === 0 ? <p className="empty-message">No emails saved yet</p> : <div className="emails-list">{emails.map((entry) => <div key={entry.id} className="email-card"><div className="email-card-clickable" onClick={() => setSelectedEmail(entry)}><div className="email-info"><p className="email"><strong>Email:</strong> {entry.email}</p><p className="account-count">{entry.accounts.length} account{entry.accounts.length !== 1 ? "s" : ""}</p></div></div><button className="delete-button" onClick={async (event) => { event.stopPropagation(); try { await removeEmail(entry.email); setEmails(emails.filter((item) => item.id !== entry.id)); } catch (error) { window.alert(String(error)); } }} title="Delete">🗑️</button></div>)}</div>}</div></div>;
}
