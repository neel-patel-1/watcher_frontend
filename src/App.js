import logo from './logo.svg';
import './App.css';
import Charts from './Chart'
import { LineChart, XAxis, YAxis, Tooltip, CartesianGrid, Line } from 'recharts';

import {React, ReactDOM, useState, useEffect} from 'react'
// const data = [
//   { time: '21:20', TCP: 100, UDP: 100, ICMP: 400 },
//   { time: '21:30', TCP: 1000, UDP: 2400, ICMP: 2400 },
//   { time: '21:40', TCP: 1000, UDP: 2400, ICMP: 2400 },
// ];
function App() {
  const [stats, setStats] = useState([
    { time: '21:40', TCP: 1000, UDP: 2400, ICMP: 2400 }
  ])
  const [isLoaded, setIsLoaded] = useState(false);
  const [error, setError] = useState(null);

  useEffect(() => {
    fetch("http://localhost:5000/packet_stats")
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
  }, [])

  if (error) {
    return <div>Error: {error.message}</div>;
  } else if (!isLoaded) {
    return <div>Loading...</div>;
  }
  else {
    return (
      <div className="App">
        <header className="App-header">
        <LineChart width={500} height={300} data={stats}>
        <XAxis dataKey="time"/>
        <Line type="monotone" dataKey="ICMP" stroke="#387908" yAxisId={2} />
        <CartesianGrid stroke="#eee" strokeDasharray="5 5"/>
        <Line type="monotone" dataKey="TCP" stroke="#8884d8" />
        <Line type="monotone" dataKey="UDP" stroke="#82ca9d" />
        </LineChart>

        
        </header>
      </div>
    );
  }
}


export default App;
