import { useState, useEffect } from "react";
import { BrowserRouter, Routes, Route } from "react-router-dom";



import Sensor from "./components/Sensor";
import System from "./components/System";

import SystemPage from "./pages/SystemPage";
import SensorPage from "./pages/SensorPage";
import LoginPage from "./pages/LoginPage";


function App() {
  
  return (
    <BrowserRouter>  
        <Routes>
          <Route exact path="/" element={<LoginPage/>} />
          <Route exact path="/" element={<SystemPage/>} />  
        </Routes> 
    </BrowserRouter>
  );
}

export default App;
