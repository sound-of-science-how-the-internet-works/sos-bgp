// Establish a Socket.io connection
const Gpio = require('onoff').Gpio;
const wardPinOut = new Gpio(27, 'out');

console.log('high');