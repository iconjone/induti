var async = require("async");
var ping = require("ping");
var arp = require("node-arp");
var dns = require("dns");
const exec = require("child_process").exec;
//var request = require("request");
var ip = require("ip");
console.log(ping);
var options = {
  // ip: '192.168.1',
  timeout: 15,
  vendor: true,
  min: 1,
  max: 255,
};

function getInfo(ip, callback) {
  var result = {
    ip: ip,
    alive: false,
    hostname: null,
    mac: null,
    vendor: null,
    hostnameError: null,
    macError: null,
    vendorError: null,
  };
  console.log(`Checking ${ip}...`);

  dns.reverse(ip, function (err, host) {
    if (err) {
      result.hostnameError = "Error on get hostname";
    } else {
      result.hostname = host && host.length ? host[0] : null;
    }
    arp.getMAC(ip, function (err2, mac) {
      if (err2 || !mac) {
        result.macError = "Error on get Mac address";
      } else {
        result.mac = mac.replace(/:([^:]{1}):/g, ":0$1:");
      }
      callback(null, result);
    });
  });
}

function getBaseIp(opts) {
  if (!("ip" in opts)) {
    var ipAddress = ip.address();
    if (ipAddress) {
      var aIp = ipAddress.split(".");
      if (aIp.length === 4) {
        opts.ip = aIp.slice(0, -1).join(".");
        return opts;
      }
    }
  } else {
    var _aIp = opts.ip.split(".");
    if (_aIp.length === 3) {
      return opts;
    } else {
      throw new Error("IP should be xxx.xxx.xxx");
    }
  }
  throw new Error("No IP address");
}
function scan(opts, callback) {
  var finalOpts = getBaseIp(opts);
  Object.assign(options, finalOpts);
  // console.log(options);
  var aIps = [];
  for (var i = options.min; i < options.max; i++) {
    aIps.push(options.ip + "." + i);
  }
  async.map(aIps, getInfo, function (err, results) {
    callback(err, results);
  });
}

function getIhIp() {
  return new Promise(function (resolve) {
    scan({}, (err, arr) => {
      console.log("I think you are runningd");
      arr = arr.filter((device) => {
        return device.hostname == "ESP-043D0F.lan";
      });
      console.log(arr);
      // if (err || arr.length != 1) resolve("0.0.0.0");
      // else resolve(arr[0].ip);
      if (arr.length == 1) {
        exec("ping " + arr[0].ip, (e, o, dr) => {
          console.log(e, o, dr);
          if (!o.includes("Destination host unreachable.")) arr[0].alive = true;
          resolve([err, arr]);
        });
      } else resolve([err, arr]);
    });
  });
}

module.exports = {
  getIhIp,
};
