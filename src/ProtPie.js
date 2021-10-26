import React from 'react'
import { ResponsiveContainer, Pie, PieChart, Cell } from 'recharts';

const ProtPie = ({data}) => {
    const COLORS = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042'];
    return (
        <div>
            <ResponsiveContainer  width={400} height={400} className='recharts-wrapper'>
            <PieChart width={1000} height={400}>
          <Pie
            dataKey="value"
            isAnimationActive={false}
            data={data}
            cx={200}
            cy={200}
            outerRadius={80}
            fill="#8884d8"
            label
          >
            {data.map((entry, index) => (
            <Cell key={`cell-${index}`} fill={COLORS[index % COLORS.length]} />
            ))}
            </Pie>
          </PieChart>
        </ResponsiveContainer>
        </div>
    )
}

export default ProtPie
