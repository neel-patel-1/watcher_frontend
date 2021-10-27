import React from 'react'
import { ResponsiveContainer, PieChart, AreaChart, XAxis, YAxis, Tooltip, CartesianGrid, Pie, Area,  Cell } from 'recharts';
import './PackData.css'
import ProtLine from './ProtLine';
import ProtPie from './ProtPie';
const PackData = ({stats}) => {
  console.log(stats[stats.length-1]['UDP'])
  const lastTime = stats[stats.length-1]
  const piedata = [{name: 'UDP', value: lastTime['UDP']}, {name: 'TCP', value: lastTime['TCP']}, {name: 'ICMP', value: lastTime['ICMP']}]
  const fstChartProts = ["TCP", "UDP", "HTTP", "ICMP", "SSL", "DNS"]
    return (
        <div >
          <ProtLine data={stats} prots={fstChartProts} />
          <ProtPie data={piedata} />
        {/* <ResponsiveContainer class='charts'  width='25%' height={400}>
        <AreaChart width={500} height={300} data={stats}
        margin = {{
          right: 0,
          left: 0,
          top: 0,
          bottom: 0,
        }}>
        <defs>
          <linearGradient id="colorhttp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#8884d8" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#8884d8" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv4" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#82cc46" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#82cc46" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="coloripv6" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#82ca9d" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#82ca9d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorudp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#8883a5" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#8883a5" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colortcp" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#88862a" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#88862a" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colordns" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#888c5d" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#888c5d" stopOpacity={0}/>
          </linearGradient>
          <linearGradient id="colorssl" x1="0" y1="0" x2="0" y2="1">
            <stop offset="0%" stopColor="#88823c" stopOpacity={0.5}/>
            <stop offset="95%" stopColor="#88823c" stopOpacity={0}/>
          </linearGradient>
        </defs>
        <XAxis dataKey="time"/>
        <YAxis/>
        <CartesianGrid stroke="#eee" strokeDasharray="5 5"/>
        <Tooltip />
        
        <Area type="monotone" dataKey="HTTP" stroke="#8884d8" fillOpacity={1} fill="url(#colorhttp)" />
        <Area type="monotone" dataKey="UDP" stroke="#82ca9d" fillOpacity={1} fill="url(#colorudp)" />
        
        <Area type="monotone" dataKey="SSL" stroke="#82ca9d" fillOpacity={1} fill="url(#colorssl)" />
        <Area type="monotone" dataKey="DNS" stroke="#82ca9d" fillOpacity={1} fill="url(#colordns)" />
        <Tooltip />
        </AreaChart>
        </ResponsiveContainer> */}
        
        </div>
    )
}

export default PackData
