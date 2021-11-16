import logo from './logo.svg';
import './App.css';

import Filter from './Filter'
import PackData from './PackData'
import SideBar from './SideBar'
import Home from './pages/Home'
import Stats from './pages/Stats'
import {React, ReactDOM, useState, useEffect} from 'react'
import { BrowserRouter as Router, Switch, Route } from 'react-router-dom';
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
    var xhr = new XMLHttpRequest();
    xhr.open("POST","http://localhost:5000/PacketStats", true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.send(JSON.stringify([1]));
    setTimeout( async () => {fetch("http://localhost:5000/PacketStats")
      .then(res => res.json())
      .then(
        (result) => {
          setInterval( async () => {fetch("http://localhost:5000/PacketStats")
          .then(res => res.json())
          .then(
            (result) => {
              setIsLoaded(true)
              setStats(result)
              var xhr = new XMLHttpRequest();
              xhr = new XMLHttpRequest();
              xhr.open("POST","http://localhost:5000/PacketStats", true);
              xhr.setRequestHeader('Content-Type', 'application/json');
              xhr.send(JSON.stringify([1]));
            },
            (error) => {
              setIsLoaded(true)
              setError(error)
            }
      )},5000)
          setIsLoaded(true)
          setStats(result)
        },
        (error) => {
          setIsLoaded(true)
          setError(error)
        }
      )
    },500)
  }, [])

  if (error) {
    return <div>Error: {error.message}</div>;
  } else if (!isLoaded) {
    return <div>Loading...</div>;
  }
  else {
    return (
      <header class="App-header">
        <Router>
        <SideBar/>
        <Switch>
          <Route path='/' exact component={Home} />
          <Route path='/reports' component={Stats} />
         
        </Switch>
        <PackData style={{
            position: 'absolute', left: '50%', top: '50%',
            transform: 'translate(-50%, -50%)'
        }} stats={stats}/>
        <Filter></Filter>
        </Router>
      </header>
    );
  }
  
}


export default App;
