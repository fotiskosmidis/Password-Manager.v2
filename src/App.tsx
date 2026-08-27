import { useEffect, useState } from "react";
import "./App.css";
import LoginScreen from "./components/LoginScreen";
import CreatePasswordScreen from "./components/CreatePasswordScreen";
import MainWindow from "./components/MainWindow";
import { keyFileExists } from "./api";

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [hasKeyFile, setHasKeyFile] = useState<boolean | null>(null);

  useEffect(() => {
    keyFileExists().then(setHasKeyFile).catch(() => setHasKeyFile(false));
  }, []);

  if (hasKeyFile === null) return <div className="app" />;

  return (
    <div className="app">
      {isLoggedIn ? (
        <MainWindow />
      ) : hasKeyFile ? (
        <LoginScreen onLogin={() => setIsLoggedIn(true)} />
      ) : (
        <CreatePasswordScreen onCreated={() => { setHasKeyFile(true); setIsLoggedIn(true); }} />
      )}
    </div>
  );
}

export default App;
