<template>
  <v-app>
    <v-app-bar app color="primary" dark>
      <div class="d-flex align-center">
        <v-img
          alt="Induti Logo"
          class="shrink mr-2"
          contain
          src="./assets/induti_logo.png"
          transition="scale-transition"
          width="35"
        />
      </div>
      <h3 :v-if="temperature != null">
        {{ convertRawTempToReal(temperature).toFixed(2) }}°F
      </h3>

      <v-spacer></v-spacer>
      <v-tooltip bottom>
        <template v-slot:activator="{ on, attrs }">
          <v-btn
            small
            fab
            @click="startCalibration()"
            color="secondary"
            v-bind="attrs"
            v-on="on"
          >
            <v-icon
              >mdi-{{
                isInserted
                  ? "bottle-soda-classic"
                  : "bottle-soda-classic-outline"
              }}</v-icon
            >
          </v-btn>
        </template>
        <span>Calibrate</span>
      </v-tooltip>

      <v-tooltip bottom>
        <template v-slot:activator="{ on, attrs }">
          <v-btn text @click="scan()" v-bind="attrs" v-on="on">
            <span class="mr-2">{{ hostAddress }}</span>
            <v-icon :key="update">
              {{ connectionIcon }}
            </v-icon>
          </v-btn>
        </template>
        <span>Scan</span>
      </v-tooltip>
    </v-app-bar>

    <v-dialog v-model="calibrationDialog" transition="dialog-bottom-transition">
      <v-card>
        <v-card-title class="headline">
          Calibrate
          {{
            `${
              calibrationSettings[calibrationStepper - 1].empty
                ? "Empty"
                : "Filled"
            }`
          }}
        </v-card-title>
        <v-stepper v-model="calibrationStepper">
          <v-stepper-header>
            <v-stepper-step
              v-for="(setting, index) in calibrationSettings"
              :key="index"
              :complete="calibrationStepper > index + 1"
              :step="index + 1"
            >
              {{
                `${setting.empty ? "Empty" : "Filled"} Calibration ${
                  setting.heat
                }`
              }}
            </v-stepper-step>
          </v-stepper-header>

          <v-stepper-items>
            <v-stepper-content
              v-for="(setting, index) in calibrationSettings"
              :key="index"
              :complete="calibrationStepper > index + 1"
              :step="index + 1"
            >
              <MeasureCalibration
                v-bind:ih="ih"
                v-bind:led="led"
                v-bind:therm="therm"
                v-bind:maxTime="setting.maxTime"
                v-bind:coolDownTime="setting.coolDownTime"
                v-bind:setting="setting"
                @calibrationDone="handleCalibrationDone"
              ></MeasureCalibration>
            </v-stepper-content>
          </v-stepper-items>
        </v-stepper>
        <v-divider></v-divider>

        <v-card-actions>
          <v-spacer></v-spacer>
          <v-btn color="primary" text @click="calibrationDialog = false">
            Cancel
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-dialog>

    <v-main>
      <v-container fluid>
        <v-row class="text-center">
          
          <v-tabs vertical color="error" :value="verticalTab" >
            <v-tab>
              <v-icon left> mdi-controller-classic </v-icon>
              Custom Control
            </v-tab>
            <v-tab>
              <v-icon left> mdi-cannabis </v-icon>
              Compounds & Terpenes
            </v-tab>
            <v-tab>
              <v-icon left> mdi-access-point </v-icon>
              Option 3
            </v-tab>

            <v-tab-item>
              <v-card flat>
                <CustomControl v-bind:ih="ih" v-bind:led="led" v-bind:isInserted="isInserted"></CustomControl>
              </v-card>
            </v-tab-item>
            <v-tab-item>
              <v-card flat>
             OK
              </v-card>
            </v-tab-item>
            <v-tab-item>
              <v-card flat>
                <v-card-text>
                  <p>
                    Fusce a quam. Phasellus nec sem in justo pellentesque
                    facilisis. Nam eget dui. Proin viverra, ligula sit amet
                    ultrices semper, ligula arcu tristique sapien, a accumsan
                    nisi mauris ac eros. In dui magna, posuere eget, vestibulum
                    et, tempor auctor, justo.
                  </p>

                  <p class="mb-0">
                    Cras sagittis. Phasellus nec sem in justo pellentesque
                    facilisis. Proin sapien ipsum, porta a, auctor quis, euismod
                    ut, mi. Donec quam felis, ultricies nec, pellentesque eu,
                    pretium quis, sem. Nam at tortor in tellus interdum
                    sagittis.
                  </p>
                </v-card-text>
              </v-card>
            </v-tab-item>
          </v-tabs>
        </v-row>
      </v-container>
      <router-view />
    </v-main>
  </v-app>
</template>

<script>
const five = require("johnny-five");
var EtherPortClient = require("./EtherPortClient").EtherPortClient;
const percentToHex = require("percent-to-hex");

const Store = require("electron-store");
const network = require("./network");
const debounce = require("lodash/debounce");

import MeasureCalibration from "./components/MeasureCalibration.vue";
import CustomControl from "./components/CustomControl.vue";
export default {
  name: "App",
  components: { MeasureCalibration, CustomControl },

  data: () => ({
    update: 0,
    board: null,
    renderComponent: true,
    electronStore: null,
    connectionIcon: "mdi-access-point-network-off",
    EPC_status: closed,
    hostAddress: "Initializing...",
    pingBoardInterval: null,
    EPC: null,
    spdt: null,
    led: null,
    therm: null,
    ih: null,
    isInserted: false,
    verticalTab: 1,
    calibrationDialog: false,
    calibrationStepper: 1,
    calibrationInfo: { therm: [120, 148, 162] },
    calibrationData: [
      {
        time: [14921, 17334],
        temperature: 123.95,
        coolDownTemperature: 115.87,
      },
      {
        time: [9609, 11495],
        temperature: 141.32,
        coolDownTemperature: 137.88,
      },
      {
        time: [10915, 12800],
        temperature: 162,
        coolDownTemperature: 156.85,
      },
      {
        time: [15490, 18510],
        temperature: 136.33,
        coolDownTemperature: 133.58,
      },
      {
        time: [10427, 12852],
        temperature: 145.8,
        coolDownTemperature: 143.88,
      },
      {
        time: [9943, 12660],
        temperature: 154.823,
        coolDownTemperature: 152.105,
      },
    ],
    calibrationDataCollection: [],
    calibrationSettings: [
      { heat: "Low", empty: true, maxTime: 25000, coolDownTime: 30000 },
      { heat: "Medium", empty: true, maxTime: 15000, coolDownTime: 45000 },
      { heat: "High", empty: true, maxTime: 15000, coolDownTime: 360000 },
      { heat: "Low", empty: false, maxTime: 20000, coolDownTime: 30000 },
      { heat: "Medium", empty: false, maxTime: 15000, coolDownTime: 45000 },
      { heat: "High", empty: false, maxTime: 15000, coolDownTime: 5000 },
    ],
    temperatureLUT: {},
    temperature: null,
  }),
  methods: {
    check: function () {
      console.log(this.electronStore);
    },
    startConnection: function () {
      this.EPC = new EtherPortClient({
        host: this.electronStore.get("hostAddress"), // IP of ESP8266 board
        port: 3030,
        callback: this.EPCmessageHandler,
      });

      //this.electronStore.set("EPC", EPC);
      this.board = new five.Board({
        port: this.EPC,
        timeout: 10000,
        repl: false,
      });
      this.board.on("ready", () => {
        this.spdt = new five.Switch(12); // need to change to different GPIO pin
        this.led = new five.Led.RGB({
          pins: [5, 4, 0],
        });
        this.therm = new five.Thermometer({
          controller: "LM35",
          pin: "A0",
        });
        this.therm.freq = 75;
        //this.therm.disable();
        this.ih = new five.Motor(14);
        console.log(this.spdt, this.led, this.therm, this.ih);
      });

      clearInterval(this.pingBoardInterval);
      this.setUpBoard();
    },
    EPCmessageHandler: function (message) {
      console.log(message);
      if (message.includes("electronStoreEtherport")) {
        this.EPC_status = message.split("_")[1];
        this.connectionIconHandler();
        if (message.split("_")[1] != "connect") {
          console.log(this.EPC);
          this.EPC.destroy();
          this.EPC = null;
          this.board = null;
          this.spdt = null;
          this.spdt = null;
          this.led = null;
          this.therm = null;
          this.ih = null;
          this.electronStore.set("hostAddress", "0.0.0.0");
          this.hostAddress = "Scanning...";
          this.connectionIcon = "mdi-magnify";
          this.scan();
        }
      }
    },
    connectionIconHandler: function () {
      console.log("update");
      // eslint-disable-next-line
      this.update += 1;
      console.log(this.EPC_status, this.hostAddress);
      this.connectionIcon =
        this.EPC_status == "connect" && this.hostAddress != undefined
          ? "mdi-access-point-check"
          : "mdi-access-point-network-off";
      console.log(this.connectionIcon);
    },
    heat: function () {
      let i = 0;
      const spdt = new five.Switch(2);
      const led = new five.Led.RGB({
        pins: [5, 4, 0],
      });
      const therm = new five.Thermometer({
        controller: "LM35",
        pin: "A0",
      });
      //therm.freq = 1000;
      therm.disable();
      let ih = new five.Motor(14);
      //ih.stop()
      spdt.on("close", () => {
        console.log("Dynavap detected" + i);
        //ih.start(50);
        led.color("#8f009c");
        i++;
      });
      spdt.on("open", () => {
        console.log("Dynavap left" + i);
        led.color("#d10d55");
        //ih.stop();
        i++;
      });

      therm.on("data", function () {
        console.log("fahrenheit: %d", this.F);
      });
      let motorspeed = 25;
      let iteration = 0;
      let heatcycle = setInterval(() => {
        ih.start(motorspeed);
        if (motorspeed > 255) motorspeed = 255;
        iteration++;
        if (motorspeed < 255) {
          motorspeed += 10;
          console.log(motorspeed);
        }
        if (iteration > 25) {
          ih.stop();
          clearInterval(heatcycle);
          led.off();
          return;
        }
        led.color(percentToHex([iteration / 25, 85, 2]));
      }, 500);

      //notes:
      //The Induction heater wants about... 25 pwm (9.8%) to 255pwm (100%) at abour 4.3s at 10 pwm steps. To convert pwm to percentage of power it is simply pwm/255
      //I beleived the induction heater will require less time if it is actually linear or expo as it will get hotter faster (3.5-4 Seconds) - Some fast hits!
      //When calibrating on empty, you can kinda see the deviation, and so you can compare how much it needs to adjust the
      //You could defintetly sell this.... if you packaged it up correctly.... All you have to do is introduce the base module.. and then you can show the connectors.. like being able to connect a scrren for battery etx
      //kinda wierd bc you're building a vape so idk.. start as a desktop accessvory I feel like
      //maybe somethign that makes the dynavap look really good looking..
      //IF you make this something AMAZING
      //You might be able to make it kick started.....
      //You can also make the code open source so sure people can build their own, but support us and we'll make it for you, esp if you don't want to have to pay for all of the pieces.. we'll get it for you :)
    },
    scan: function () {
      network.getIhIp().then((results) => {
        console.log(results);
        let arr = results[1];
        if (results[0]) {
          //err
          //Something went wrong
          this.electronStore.set("hostAddress");
          this.hostAddress = "Can't find your device";
        } else if (arr.length != 1) {
          this.electronStore.delete("hostAddress");
          this.hostAddress = "Multiple Devices found on network...";
        } else if (!arr[0].alive) {
          this.electronStore.delete("hostAddress");
          this.hostAddress =
            "Device turned off. Please turn on and Scan Again.";
        } else {
          this.electronStore.set("hostAddress", arr[0].ip);
          this.hostAddress = arr[0].ip;
          console.log("should only run after");
          this.startConnection();
          this.renderComponent = false;
          this.renderComponent = true;
        }
      });
    },
    setUpBoard: function () {
      // eslint-disable-next-line
      let vueApp = this;
      this.board.on("ready", () => {
        this.spdt.on(
          "close",
          debounce(() => {
            this.isInserted = true;
          }, 150)
        );
        this.spdt.on(
          "open",
          debounce(() => {
            this.isInserted = false;
          }, 150)
        );

        this.therm.on("change", function () {
          // console.log("fahrenheit: %d", this.F);
          if (this.F > vueApp.calibrationInfo.therm[0])
            vueApp.temperature = this.F;
          else vueApp.temperature = vueApp.calibrationInfo.therm[0];
        });
        console.log("ran pinging");
        const led = new five.Led(16);
        this.pingBoardInterval = setInterval(() => {
          led.toggle();
        }, 1000);
      });
    },
    convertRawTempToReal: function (raw) {
      if (Object.keys(this.temperatureLUT).length != 0) {
        if (raw < this.temperatureLUT["70"]) return 70;
        else if (this.temperatureLUT["70"] <= raw < this.temperatureLUT["90"])
          return this.calculatePoint(
            this.temperatureLUT["70"],
            70,
            this.temperatureLUT["90"],
            90,
            raw
          );
        else if (this.temperatureLUT["90"] <= raw < this.temperatureLUT["250"])
          return this.calculatePoint(
            this.temperatureLUT["90"],
            90,
            this.temperatureLUT["250"],
            250,
            raw
          );
        else if (this.temperatureLUT["250"] <= raw < this.temperatureLUT["400"])
          return this.calculatePoint(
            this.temperatureLUT["250"],
            250,
            this.temperatureLUT["400"],
            400,
            raw
          );
        else if (this.temperatureLUT["400"] <= raw < this.temperatureLUT["430"])
          return this.calculatePoint(
            this.temperatureLUT["400"],
            400,
            this.temperatureLUT["430"],
            430,
            raw
          );
        else
          return this.calculatePoint(
            this.temperatureLUT["430"],
            430,
            this.temperatureLUT["450"],
            450,
            raw
          );
      } else {
        return 70;
      }
    },
    calculatePoint: function (x1, y1, x2, y2, raw) {
      let m = (y2 - y1) / (x2 - x1);
      let b = y2 - m * x2;
      return m * raw + b;
    },
    startCalibration: function () {
      if (this.boardConnection) {
        this.calibrationDialog = true;
      }
    },
    handleCalibrationDone: function (data) {
      this.calibrationDataCollection.push(data);
      if (this.calibrationStepper < 6)
        this.calibrationStepper = this.calibrationStepper + 1;
      else {
        this.calibrationDialog = false;
        this.electronStore.set(
          "calibrationData",
          this.calibrationDataCollection
        );
        this.calibrationData = this.calibrationDataCollection;
        this.initiateCustomMeasurements();
      }
    },
    initiateCustomMeasurements: function () {
      this.temperatureLUT = {
        450:
          Math.max(
            this.calibrationData[2].temperature,
            this.calibrationData[5].temperature
          ) +
          (this.calibrationData[5].temperature -
            this.calibrationData[4].temperature),
        430:
          (this.calibrationData[2].temperature +
            this.calibrationData[5].temperature) /
          2,
        400:
          (this.calibrationData[4].temperature +
            this.calibrationData[3].coolDownTemperature) /
          2,
        250:
          (this.calibrationData[0].temperature +
            this.calibrationData[3].coolDownTemperature) /
          2,
        90: this.calibrationData[0].temperature,
        70: this.calibrationData[0].coolDownTemperature,
      };
    },
  },
  computed: {
    boardConnection: function () {
      return this.EPC_status == "connect" && this.board != null;
    },
  },
  created: function () {
    console.log("Vue Instance", this);
    this.electronStore = new Store();

    if (this.electronStore.get("hostAddress") == undefined) {
      this.scan();
    } else {
      this.hostAddress = this.electronStore.get("hostAddress");
      this.startConnection();
    }

    if (this.electronStore.get("calibrationData") != undefined) {
      this.calibrationData = this.electronStore.get("calibrationData");
    }
    if(this.electronStore.get("verticalTab") != undefined){
      this.verticalTab = this.electronStore.get("verticalTab")
    }

    this.initiateCustomMeasurements();

    const mq = window.matchMedia("(prefers-color-scheme: dark)");

    this.$vuetify.theme.dark = mq.matches;

    mq.addEventListener("change", (e) => {
      this.$vuetify.theme.dark = e.matches;
    });
    //this.findHostAddress();
    //   if (this.electronStore.get("hostAddress") == undefined) {
    //     console.log('finding')
    //     this.findHostAddress();
    //   } else
    //   {
    //   console.log('running')

    // }
  },
  watch:{
    verticalTab:function(val){
      this.electronStore.set("verticalTab", val)
    }
  },
};
</script>
<style>
html {
  overflow-y: auto;
}
</style>
