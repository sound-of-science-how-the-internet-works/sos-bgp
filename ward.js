// Establish a Socket.io connection
const feathers = require('@feathersjs/feathers');
const socketio = require('@feathersjs/socketio-client');
const io = require('socket.io-client');

const socket = io('http://192.168.0.11:3030');
const client = feathers();

const Gpio = require('onoff').Gpio;
const debug = true;

// Initialize our Feathers client application through Socket.io
// with hooks and authentication.

client.configure(socketio(socket));

// send
const wardPinIn = new Gpio(17, 'in', 'rising');
const wardPinOut = new Gpio(27, 'out');

let shouldCheck = true;
wardPinIn.watch(function (err, value) {
    if (debug) {
        console.log('new');// Watch for hardware interrupts on pushButton GPIO, specify callback function
    }
    if (!shouldCheck) {
        return;
    }
    if (err) { //if an error
        console.error('There was an error', err); //output error message to console
    }
    if (debug) {
        console.log(value);
    }

    client.service('states').patch(2, {
        valid: true
    });
});
let resetSended = false;
client.service('states').on('patched', data => {
    if (!resetSended && data.hasOwnProperty('panel') && data.hasOwnProperty('valid') && data.panel === 'dns' && data.valid === true) {
        if (debug) {
            console.log('sending');
        }
        wardPinOut.write(1);
        resetSended = true;
        shouldCheck = true;
        setTimeout(function () {
            if (debug) {
                console.log('low');
            }
            wardPinOut.write(0);
            client.service('states').patch(2, {
                valid: false
            });
            shouldCheck = false;
        }, 5000);
    }
});