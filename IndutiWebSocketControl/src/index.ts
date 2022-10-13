import WebSocket = require('isomorphic-ws');
var EventEmitter = require('events')
//Receive WS url and create WebSocket instance that can send messages and update a variable
//on close, reconnect to the server
class IndutiWebSocketControl {
  public ip: string;
  public ws: any;
  public state: { IH: number; LED: number; CONTACT: boolean; TEMP: number };
  public isConnected: boolean;
  public timer: any;
  public stateToSet: { IH: any; LED: any; };
  public event : any;
  public options: {test: boolean};

  constructor(inputIP?: string, options?: { test: boolean }) {
    this.ip = inputIP || ""; //Use MDNS to get IP of induti.local if no IP is given
    this.options = options || { test: false };
    this.ws = null;
    this.isConnected = false;
    this.state = { IH: 0, LED: 0, CONTACT: false, TEMP: 0 };
    this.stateToSet = { IH: null, LED: null };
    this.timer = setTimeout(() => { this.processSend }, 50);
    this.event = new EventEmitter();
    this.connect();
  }

  connect() {
    //every 5 seconds emit an event with the current state

    this.ws = new WebSocket(`ws://${this.ip}/ws`);
    
    this.ws.onopen = () => {
      this.isConnected = true;
      this.event.emit('connection', this.isConnected)
    };
    this.ws.onmessage = (event: any) => {
      // console.log(event.data);
      const data = JSON.parse(event.data);
      if (data.status == undefined) {
        this.state.IH = data.IH;
        this.state.LED = data.LED;
        this.state.CONTACT = !data.CONTACT;
        this.state.TEMP = data.TEMP;
       this.event.emit('data', this.state)
      } else if (data.status == false) {
        console.log('Command Failed');
      }
    };
    this.ws.onclose = () => {
      //Allow closing of websocket if this.close() is called
      this.isConnected = false;
      this.event.emit('connection', this.isConnected)
      this.connect();
    };
    this.ws.onerror = (error: any) => {
      // console.log(error);
      this.isConnected = false;
      this.connect();
    }

  }
  close() {
    this.ws.close();
  }
  send(message: { type: string; IH?: number; LED?: number }) {
    if (this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(message));
    } else {
      console.log('WebSocket is not connected');
      //resend message once the connection is reestablished
      setTimeout(() => {
        this.send(message);
      }  , 1000);
    }
  }
  sendIH(pwm: number) {
    if (this.isConnected) {
      this.send({ type: 'I', IH: pwm });
    } else {
      console.log('WebSocket is not connected');
    }
  }
  sendL(hex: number) {
    if (this.isConnected) {
      this.send({ type: 'L', LED: hex });
    } else {
      console.log('WebSocket is not connected');
    }
  }
  sendLIH(IH: number, L: number) {
    if (this.isConnected) {
      this.send({ type: 'IL', IH: IH, LED: L });
    } else {
      console.log('WebSocket is not connected');
    }
  }

  setIH(pwm: number) {
    this.stateToSet.IH = pwm;
    this.processSend();
  }
  setL(hex: number) {
    this.stateToSet.LED = hex;
    this.processSend();
  }
  setLIH(IH: number, L: number) {
    this.stateToSet.IH = IH;
    this.stateToSet.LED = L;
    this.processSend();
  }



  processSend() {
    let timer: any;

    if (!timer) {
      //take the stateToSet and depending on if IH or LED is defined, send it to the server
      //if both are defined, send both and use sendLIH to send it to the server
      if (this.stateToSet.IH !== null && this.stateToSet.LED !== null) {
        this.sendLIH(this.stateToSet.IH, this.stateToSet.LED);
        //Set the stateToSet to null so that it doesn't send the same command again
        this.stateToSet.IH = null;
        this.stateToSet.LED = null;
      }
      else if (this.stateToSet.IH !== null) {
        this.sendIH(this.stateToSet.IH);
        //Set the stateToSet to null so that it doesn't send the same command again
        this.stateToSet.IH = null;
      }
      else if (this.stateToSet.LED !== null) {
        this.sendL(this.stateToSet.LED);
        //Set the stateToSet to null so that it doesn't send the same command again
        this.stateToSet.LED = null;
      }

      //Start the timer to run processSend again after 1 second
      timer = setTimeout(() => {
        timer = null;
        this.processSend();
      })

    }
    else {
      clearTimeout(timer);
    }

  };

}

export { IndutiWebSocketControl };
