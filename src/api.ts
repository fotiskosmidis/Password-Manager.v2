import { invoke } from "@tauri-apps/api/core";
import type { Account, EmailEntry } from "./types";

export const createKeyFile = (password: string) =>
  invoke<boolean>("create_key_file", { password });

export const keyFileExists = () => invoke<boolean>("key_file_exists");

export const validatePassword = (password: string) =>
  invoke<boolean>("validate_password", { password });

export const loadData = () => invoke<void>("load_data");
export const saveData = () => invoke<void>("save_data");

export const getEmails = () => invoke<string[]>("get_emails");

export const getEmailEntries = async (): Promise<EmailEntry[]> => {
  const emailAddresses = await getEmails();
  return Promise.all(emailAddresses.map(async (email, index) => ({
    id: index + 1,
    email,
    accounts: await getAccounts(email),
  })));
};
export const addEmail = (email: string) => invoke<void>("add_email", { email });
export const removeEmail = (email: string) =>
  invoke<void>("remove_email", { email });

export const addAccount = (
  email: string,
  company: string,
  password: string,
  name: string,
) => invoke<number>("add_account", { email, company, password, name });

export const getAccounts = async (email: string): Promise<Account[]> => {
  const accounts = await invoke<[number, string, string, string][]>(
    "get_accounts",
    { email },
  );
  return accounts.map(([id, company, accountName, password]) => ({
    id,
    company,
    accountName,
    password,
  }));
};

export const removeAccount = (email: string, accountId: number) =>
  invoke<void>("remove_account", { email, accountId });

export const updateAccount = (
  email: string,
  accountId: number,
  company: string,
  password: string,
  name: string,
) => invoke<void>("update_account", {
  email,
  accountId,
  company,
  password,
  name,
});
