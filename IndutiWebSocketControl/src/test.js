//Load module from lib folder

const {IndutiWebSocketControl} = require('../lib/index.js');

let induti = new IndutiWebSocketControl("localhost:8080")
//localhost:8080

induti.event.on("data", (data) => {
    console.log(data)
})

