<template>
  <v-container>
    <v-overlay :value="overlay">
      <h1>
        {{
          `${
            setting.empty
              ? setting.heat == "High" && setting.coolDownTime == 360000
                ? "Cooling Down... Fill Bowl with Material"
                : "Cooling Down..."
              : setting.heat == "High" && setting.coolDownTime == 5000
              ? "Calibrating... Take your hit."
              : "Cooling Down... Take your hit."
          }`
        }}
      </h1>
      <v-progress-linear
        color="primary"
        :value="coolDown"
        height="10"
        stream
      ></v-progress-linear>
    </v-overlay>
    <v-row>
      <v-col>
        <v-btn :disabled="heatingDisable" @click="startMeasure()">
          Start Heating
        </v-btn>
      </v-col>
      <v-col>
        <v-btn :disabled="firstDisable" @click="firstClick()">
          First Click
        </v-btn>
      </v-col>
      <v-col>
        <v-btn :disabled="secondDisable" @click="secondClick()">
          Second Click
        </v-btn>
      </v-col>
    </v-row>
    <v-row>
      <v-progress-linear
        color="primary"
        :value="progressValue"
        :buffer-value="bufferValue"
        :indeterminate="firstClickms == null ? true : false"
      ></v-progress-linear>
    </v-row>
  </v-container>
</template>

<script>
import Timer from "tiny-timer";
export default {
  name: "MeasureCalibration",

  data: () => ({
    firstClickms: null,
    secondClickms: null,
    timer: null,
    coolDown: null,
    overlay: null,
    heatingDisable: false,
    firstDisable: true,
    secondDisable: true,
    temperatureData: [],
    coolDownTemperature: [],
  }),
  props: ["ih", "therm", "setting", "led"],
  methods: {
    startMeasure: function () {
      this.ih.start(255);
      this.led.color("#ab0505");
      this.timer.start(45000);
      this.heatingDisable = true;
      this.firstDisable = false;
    },
    firstClick: function () {
      this.firstClickms = this.timer.time;
      console.log(this.firstClickms);
      this.led.color("#781a6d");
      this.firstDisable = true;
      this.secondDisable = false;
    },
    secondClick: function () {
      this.secondClickms = this.timer.time;
      console.log(this.secondClickms);
      this.timer.stop();
      this.ih.stop();
      this.secondDisable = true;
      this.initiateCoolDown();
    },
    convertToProgressValue: function (time) {
      console.log(time);
      console.log((time / this.setting.maxTime) * 100);
      return (time / this.setting.maxTime) * 100;
    },
    initiateCoolDown: function () {
      this.overlay = true;
      this.led.intensity(100);
      this.led.color("#1a781e");
      const coolDownTimer = new Timer({ interval: 125, stopwatch: true });
      coolDownTimer.start(this.setting.coolDownTime);
      coolDownTimer.on("tick", (ms) => {
        this.coolDown = (ms / coolDownTimer.duration) * 100;
        console.log(ms, (ms / coolDownTimer.duration) * 100);
      });
      coolDownTimer.on("done", () => {
        this.finishCalibrationStep();
      });
      console.log(this.temperatureData);
    },
    finishCalibrationStep: function () {
      console.log(this.temperatureData);

      let ret = {
        time: [this.firstClickms, this.secondClickms],
        temperature: this.getQ3(this.temperatureData),
        coolDownTemperature: this.getQ3(this.coolDownTemperature),
      };
      this.led.off();
      this.$emit("calibrationDone", ret);
    },
    getQ3: function (arr) {
      if (arr.length > 3) {
        return (
          arr
            .sort()
            .slice((arr.length / 4).toFixed(0))
            .reduce((sum, val) => (sum += val)) /
          (arr.length - (arr.length / 4).toFixed(0))
        );
      } else return arr.reduce((sum, val) => (sum += val)) / arr.length;
    },
  },
  computed: {
    progressValue: function () {
      console.log(this.firstClickms);
      if (!(this.firstClickms == null))
        return this.convertToProgressValue(this.firstClickms);
      else return 0;
    },
    bufferValue: function () {
      console.log(this.secondClickms, !(this.secondClickms == null));
      if (!(this.secondClickms == null))
        return this.convertToProgressValue(this.secondClickms);
      else return this.progressValue;
    },
  },
  watch: {},
  mounted() {
    console.log(this);
  },
  created() {
    // eslint-disable-next-line
      let vueApp = this;
    this.timer = new Timer({
      stopwatch: true,
    });

    this.therm.on("change", function () {
      // console.log("Temperature", this.F)
      if (vueApp.firstClickms != null && vueApp.secondClickms == null)
        vueApp.temperatureData.push(this.F);
      if (vueApp.heatingDisable && vueApp.firstClickms == null)
        vueApp.coolDownTemperature.push(this.F);
    });
  },
};
</script>
