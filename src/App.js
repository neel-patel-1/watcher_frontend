import logo from './logo.svg';
import './App.css';

import Filter from './Filter'
import PackData from './PackData'
import {React, ReactDOM, useState, useEffect} from 'react'
// const data = [
//   { time: '21:20', TCP: 100, UDP: 100, ICMP: 400 },
//   { time: '21:30', TCP: 1000, UDP: 2400, ICMP: 2400 },
//   { time: '21:40', TCP: 1000, UDP: 2400, ICMP: 2400 },
// ];
function App() {
  const [stats, setStats] = useState([
    { time: 1000, ETH: 1, IPV4: 1, IPV6: 1, UDP: 1, TCP: 1, DNS: 1, HTTP: 1, SSL: 1}
  ])
  const [isLoaded, setIsLoaded] = useState(false);
  const [error, setError] = useState(null);

  useEffect(() => {
    setInterval( async () => {fetch("http://172.18.177.104:5000/PacketStats")
      .then(res => res.json())
      .then(
        (result) => {
          setIsLoaded(true)
          setStats(result)
        },
        (error) => {
          setIsLoaded(true)
          setError(error)
        }
      )
    },5000)
  }, [])

  if (error) {
    return <div>Error: {error.message}</div>;
  } else if (!isLoaded) {
    return <div>Loading...</div>;
  }
  else {
    return (
      <header class="App-header">
        <PackData stats={stats}/>
        <Filter></Filter>
      </header>
    );
  }
  
}


export default App;
