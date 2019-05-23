// Establish a Socket.io connection
const Gpio = require('onoff').Gpio;
const wardPinOut = new Gpio(27, 'out');

console.log('high');
wardPinOut.write(1);
setTimeout(function () {
    console.log('low');
    wardPinOut.write(0);
}, 5000);