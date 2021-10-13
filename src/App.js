import logo from './logo.svg';
import './App.css';
import { LineChart, XAxis, YAxis, Tooltip, CartesianGrid, Line } from 'recharts';
import {useState, useEffect} from 'react'
const data = [
  { time: '21:20', TCP: 100, UDP: 100, ICMP: 400 },
  { time: '21:30', TCP: 1000, UDP: 2400, ICMP: 2400 },
  { time: '21:40', TCP: 1000, UDP: 2400, ICMP: 2400 },
];
function App() {
  
  useEffect(() => {
    const fetchTasks = async () => {
      const res = await fetch('http://localhost:5000/packetCount')
      const data = await res.json()

      console.log(data)
    }
  }, [])
  return (
    <div className="App">
      <header className="App-header">
      <LineChart width={500} height={300} data={data}>
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

export default App;
