import { useState } from "react";
import "./App.css";
import LoginScreen from "./components/LoginScreen";
import MainWindow from "./components/MainWindow";

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);

  return (
    <div className="app">
      {isLoggedIn ? (
        <MainWindow />
      ) : (
        <LoginScreen onLogin={() => setIsLoggedIn(true)} />
      )}
    </div>
  );
}

export default App;
