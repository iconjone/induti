//Create Websocket server
// var mdns = require('mdns')

WebSocket = require("isomorphic-ws")

const ws = new WebSocket.Server({ port: 8080 , path: "/ws"});
// ad = mdns.createAdvertisement(mdns.tcp('http'), 8080, {name: 'IndutiWebSocketControl', host: 'induti'});
// ad.start();

let state = {IH: 0, LED: 0, TEMP: 0, CONTACT: 0}
console.log("Server started")
console.log(ws)
//Every 75 ms send state to all clients
setInterval(() => {
    ws.clients.forEach(client => {
        if (client !== ws)
        client.send(JSON.stringify(state));
    });
}, 75);

//Every 25 ms update state
setInterval(() => {
    state.TEMP = Math.random() * (75 - 0.5) + 0.5;
    state.CONTACT = Math.round(Math.random())
})



//when client sends message update state
ws.on('connection', ws => {
    ws.on('message', message => {
        let data = JSON.parse(message);
        state.IH = data.IH || state.IH;
        state.LED = data.LED || state.LED;
    });
})