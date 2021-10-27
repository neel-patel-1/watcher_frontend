# Set Up:

git submodules init
git submodules update
--to fetch PcapPlusPlus

dependencies for compiling backend:
libpcap-dev
libcpprest-dev

starting the (test) frontend
type 'npm install' to get the packages for react development server
'npm start' to start the react development server (frontend)

'sudo make' from Writer Directory to compile backend
follow compilation instructions given by PcapPlusPlus
'sudo ./Writer/Bin/Writer' to start backend
OR
'npm run server' to start a test json-server

