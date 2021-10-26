import React from 'react';
import * as FaIcons from 'react-icons/fa';
import * as AiIcons from 'react-icons/ai';
import * as IoIcons from 'react-icons/io';
export const SidebarData = [
    {
      title: 'Home',
      path: '/',
      icon: <AiIcons.AiFillHome />,
      cName: 'nav-text'
    },
    {
      title: 'Statistics',
      path: '/stats',
      icon: <AiIcons.AiFillPieChart />,
      cName: 'nav-text'
    },
    {
      title: 'Filter',
      path: '/reports',
      icon: <AiIcons.AiTwotoneFilter />,
      cName: 'nav-text'
    },
    // {
    //   title: 'Products',
    //   path: '/products',
    //   icon: <FaIcons.FaCartPlus />,
    //   cName: 'nav-text'
    // },
  ];