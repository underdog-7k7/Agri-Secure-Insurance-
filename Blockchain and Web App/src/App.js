import Web3 from "web3";
import AgriSec from "./contracts/AgriSecure.json";
import {useState, useEffect} from "react";
import './App.css';
import Dashboard from './pages/dashboard/Dashboard.jsx'
import Login from './pages/login/Login.jsx'
import BlockchainView from './pages/blockchainview/BlockchainView.jsx'
import Profile from './pages/profile/Profile.jsx'
import Weather from './pages/weather/Weather.jsx'
import { Routes,Route } from "react-router-dom";


function App() {  
  const [weatherData, setWeatherData] = useState(null);
  const [stat, setStat] = useState(null);
  const [data, setData] = useState("nil");
  useEffect(() => {
    const provider = new Web3.providers.HttpProvider("HTTP://127.0.0.1:7545");

    async function template() {
      const web3 = new Web3(provider);
      const networkId = await web3.eth.net.getId();
      const deployedNetwork = AgriSec.networks[networkId];
      const contract = new web3.eth.Contract(
        AgriSec.abi,
        deployedNetwork.address
      );
      console.log(contract);
      setStat(contract);
      console.log(stat);
    }
    provider && template();
  }, []);


  useEffect(()=>{
    const fetchData = async () => {
      try {
          const response = await fetch('https://api.thingspeak.com/channels/2508245/feeds.json?api_key=L14H48B8RGX9QE07&results=1');
          const wData = await response.json();
          console.log("wData");
          console.log(wData);
          setWeatherData(wData);
          console.log(stat);
          console.log(stat);
          const data1 = Math.round(wData.feeds[0].field1);
          const data2 = Math.round(wData.feeds[0].field2);
          const data3 = Math.round(wData.feeds[0].field3);
          const data4 = Math.round(wData.feeds[0].field4);
          // const data5 = Math.round(1);
          await stat._methods
          .setter(data1, data2, data3, data4)
          .send({ from: "0x29225002E7a5A8102d60c8CD5eaaB61EC5C8F828" });
          console.log("done");

          // const bData = await stat._methods.getter().call();
          // // console.log(typeof(data[0]));
          // setData(bData);
          // console.log(bData);
      } catch (error) {
          console.error('Error fetching data:', error);
      }
    };

    // fetchData();    
    const interval = setInterval(fetchData,10000);
    return()=>clearInterval(interval);

  },[stat])
  

  return (
    <Routes>
      <Route exact path="/" element={<Login/>}></Route>
      <Route path="/dashboard" element={<Dashboard/>}></Route>
      <Route path="/profile" element={<Profile/>}></Route>
      <Route path="/blockchainview" element={<BlockchainView/>}></Route>
      <Route path="/weather" element={<Weather/>}></Route>
    </Routes>
  );
}

export default App;
