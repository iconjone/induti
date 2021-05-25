// const network = require("./network");

// console.log("running");

// // function scan() {
// //   network.getIhIp().then((results) => {
// //     console.log(results);
// //     console.log("should only run after");
// //   });
// // }

// //scan();

// const { tcpPingPort } = require("tcp-ping-port");
// tcpPingPort("192.168.1.16", 3030).then((online) => {
//   console.log(online);
// });

// function delay() {
//     // `delay` returns a promise
//     return new Promise(function(resolve, reject) {
//       // Only `delay` is able to resolve or reject the promise
//       setTimeout(function() {
//         resolve(42); // After 3 seconds, resolve the promise with value 42
//       }, 3000);
//     });
//   }

//   delay()
//     .then(function(v) { // `delay` returns a promise
//       console.log(v); // Log the value once it is resolved
//     })
//     .catch(function(v) {
//       // Or do something else if it is rejected
//       // (it would not happen in this example, since `reject` is not called).
//     });
