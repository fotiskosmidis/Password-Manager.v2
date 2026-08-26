export interface Account {
  id: number;
  company: string;
  accountName: string;
  password: string;
}

export interface EmailEntry {
  id: number;
  email: string;
  accounts: Account[];
}
