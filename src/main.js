import Vue from "vue";
import App from "./App.vue";
import router from "./router";
import store from "./store";
import vuetify from "./plugins/vuetify";
import "roboto-fontface/css/roboto/roboto-fontface.css";
import "@mdi/font/css/materialdesignicons.css";

Vue.config.productionTip = false;

new Vue({
  router,
  store,
  vuetify,

  render: function (h) {
    return h(App);
  },
  created() {
    // Prevent blank screen in Electron builds
    // this.$router.push("/");
  },
}).$mount("#app");

// const five = require("johnny-five");
// var EtherPortClient = require("etherport-client").EtherPortClient;
// var board = new five.Board({
//   port: new EtherPortClient({
//     host: "192.168.1.16", // IP ESP8266
//     port: 3030,
//   }),
//   timeout: 10000,
//   repl: true,
// });

// var i = 0;
// console.log(board, i);

// board.on("ready", () => {
//   const led = new five.Led(13);
//   const spdt = new five.Switch(8);
//   spdt.on("close", ()=>{
//     console.log('Dynavap detected' + i)
//     i++
//   })
//   spdt.on("open", ()=>{
//     console.log('Dynavap left' + i)
//     i++
//   })
// led.on()
// });
