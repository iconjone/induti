<template>
  <v-container class="mt-8">
    <v-row>
      <v-col>
        <v-slider thumb-label v-model="pwm" max="255" min="0">
          <template v-slot:append>
            <v-text-field
              v-model="pwm"
              class="mt-0 pt-0"
              type="number"
              style="width: 60px"
            ></v-text-field>
          </template>
        </v-slider>
      </v-col>
      <v-col cols="3">
        <v-switch
          v-model="toggle"
          :label="`IH: ${toggle ? 'On' : 'Off'}`"
          v-on:keyup.13="test"
        ></v-switch>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
const percentToHex = require("percent-to-hex");
export default {
  name: "CustomControl",
  data: () => ({
    pwm: 0,
    toggle: false,
  }),
  props: ["ih", "led", "isInserted"],
  created: function () {
    console.log(this);
  },
  methods: {
    test: function () {
      console.log("test");
    },
  },
  watch: {
    pwm: function (val) {
      this.led.color(percentToHex([val / 255, 85, 2]));
      if (this.toggle && this.isInserted) {
        this.ih.start(val);
      }
    },
    toggle: function (val) {
      if (!val) {
        this.ih.stop();
        this.led.stop();
        this.led.off();
      } else {
        this.led.strobe(500);
        this.ih.start(this.pwm);
      }
    },
    isInserted: function(val){
        if(!val){
            this.toggle = false
        }
    }
  },
};
</script>
