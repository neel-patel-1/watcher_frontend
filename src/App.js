import logo from './logo.svg';
import './App.css';
import { LineChart, AreaChart, XAxis, YAxis, Tooltip, CartesianGrid, Line, Area } from 'recharts';
import { ProSidebar, Menu, MenuItem, SubMenu } from 'react-pro-sidebar';
import 'react-pro-sidebar/dist/css/styles.css';
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
    setInterval( async () => {fetch("http://localhost:5000/PacketStats")
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
      <div className="App">
        
        <header className="App-header" style={{    display: "grid",  
    gridTemplateColumns: "1fr 2fr"}}>
        
        <div>
        <ProSidebar >
          <Menu iconShape="square">
            <MenuItem >Dashboard</MenuItem>
            <SubMenu title="Components" >
              <MenuItem>Component 1</MenuItem>
              <MenuItem>Component 2</MenuItem>
            </SubMenu>
          </Menu>
        </ProSidebar >
        </div>
        <div >
        <LineChart width={500} height={300} data={stats}>
        <XAxis dataKey="time"/>
        <YAxis/>
        {/* <Line type="monotone" dataKey="ICMP" stroke="#387908" /> */}
        <CartesianGrid stroke="#eee" strokeDasharray="5 5"/>
        <Line type="monotone" dataKey="ETH" stroke="#8884d8" />
        {/* <Line type="monotone" dataKey="UDP" stroke="#82ca9d" /> */}
        <Tooltip />
        </LineChart>
        <AreaChart width={500} height={300} data={stats}>
        <defs>
          <linearGradient id="colorhttp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#8884d8" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#8884d8" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv4" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#82cc46" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#82cc46" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv6" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#82ca9d" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#82ca9d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorudp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#8883a5" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#8883a5" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colortcp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#88862a" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#88862a" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colordns" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#888c5d" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#888c5d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorssl" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#88823c" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#88823c" stopOpacity={0}/>
          </linearGradient>
        </defs>
        <XAxis dataKey="time"/>
        <YAxis/>
        <CartesianGrid stroke="#eee" strokeDasharray="5 5"/>
        <Tooltip />
        <Area type="monotone" dataKey="TCP" stroke="#8884d8" fillOpacity={1} fill="url(#colortcp)" />
        <Area type="monotone" dataKey="HTTP" stroke="#8884d8" fillOpacity={1} fill="url(#colorhttp)" />
        <Area type="monotone" dataKey="UDP" stroke="#82ca9d" fillOpacity={1} fill="url(#colorudp)" />
        <Area type="monotone" dataKey="IPV4" stroke="#82ca9d" fillOpacity={1} fill="url(#coloripv4)" />
        <Area type="monotone" dataKey="IPV6" stroke="#82ca9d" fillOpacity={1} fill="url(#coloripv6)" />
        {/* <Area type="monotone" dataKey="ETH" stroke="#82ca9d" fillOpacity={1} fill="url(#coloreth)" /> */}
        <Area type="monotone" dataKey="SSL" stroke="#82ca9d" fillOpacity={1} fill="url(#colorssl)" />
        <Area type="monotone" dataKey="DNS" stroke="#82ca9d" fillOpacity={1} fill="url(#colordns)" />
        <Tooltip />
        </AreaChart>
        <AreaChart width={500} height={300} data={stats}>
        <defs>
          <linearGradient id="colorhttp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#8884d8" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#8884d8" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv4" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#82cc46" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#82cc46" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv6" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#82ca9d" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#82ca9d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorudp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#8883a5" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#8883a5" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colortcp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#88862a" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#88862a" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colordns" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#888c5d" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#888c5d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorssl" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#88823c" stopOpacity={0.8}/>
            <stop offset="95%" stopColor="#88823c" stopOpacity={0}/>
          </linearGradient>
        </defs>
        <XAxis dataKey="time"/>
        <YAxis/>
        <CartesianGrid stroke="#eee" strokeDasharray="5 5"/>
        <Tooltip />
        {/* <Area type="monotone" dataKey="TCP" stroke="#8884d8" fillOpacity={1} fill="url(#colortcp)" /> */}
        <Area type="monotone" dataKey="HTTP" stroke="#8884d8" fillOpacity={1} fill="url(#colorhttp)" />
        <Area type="monotone" dataKey="UDP" stroke="#82ca9d" fillOpacity={1} fill="url(#colorudp)" />
        {/* <Area type="monotone" dataKey="IPV4" stroke="#82ca9d" fillOpacity={1} fill="url(#coloripv4)" /> */}
        {/* <Area type="monotone" dataKey="IPV6" stroke="#82ca9d" fillOpacity={1} fill="url(#coloripv6)" /> */}
        {/* <Area type="monotone" dataKey="ETH" stroke="#82ca9d" fillOpacity={1} fill="url(#coloreth)" /> */}
        <Area type="monotone" dataKey="SSL" stroke="#82ca9d" fillOpacity={1} fill="url(#colorssl)" />
        <Area type="monotone" dataKey="DNS" stroke="#82ca9d" fillOpacity={1} fill="url(#colordns)" />
        <Tooltip />
        </AreaChart>
        
        </div>
        
</header>
        </div>
    );
  }
}


export default App;
