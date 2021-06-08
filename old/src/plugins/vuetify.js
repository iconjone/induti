import Vue from "vue";
import Vuetify from "vuetify/lib/framework";

Vue.use(Vuetify);

export default new Vuetify({
  theme: {
    themes: {
      light: {
        primary: "#ab0505",
        secondary: "#2e2e2e",
        accent: "#82B1FF",
        error: "#FF5252",
        info: "#2196F3",
        success: "#1a781e",
        warning: "#FFC107",
      },
      dark: {
        primary: "#ab0505",
        secondary: "#2e2e2e",
      },
    },
  },
});
