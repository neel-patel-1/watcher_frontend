import logo from './logo.svg';
import './App.css';
import { LineChart, XAxis, Tooltip, CartesianGrid, Line } from 'recharts';
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
      <LineChart
        width={300}
        height={400}
        data={data}
        margin={{ top: 10, right: 20, left: 10, bottom: 5 }}
      >
        <XAxis dataKey="time" />
        <Tooltip />
        <CartesianGrid stroke="#f5f5f5" />
        <Line type="monotone" dataKey="TCP" stroke="#ff7300" yAxisId={0} />
        <Line type="monotone" dataKey="UDP" stroke="#387908" yAxisId={1} />
        <Line type="monotone" dataKey="ICMP" stroke="#387908" yAxisId={2} />
      </LineChart>
      </header>
    </div>
  );
}

export default App;
