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
const wardPinOut = new Gpio(22, 'out');
wardPinOut.write(1);

let shouldCheck = true;
if (debug) {
    console.log('Started');
}
wardPinIn.watch(function (err, value) {
    if (debug) {
        console.log('WARD PIN');
        console.log(shouldCheck);
    }
    if (!shouldCheck) {
        return;
    }
    if (err) { //if an error
        console.error('There was an error', err); //output error message to console
    }
    if (debug) {
        console.log('new value');
        console.log(value);
    }

    client.service('states').patch(2, {
        valid: true
    });
    shouldCheck = false;
});
let resetSended = false;
client.service('states').on('patched', data => {
    if (!resetSended && data.hasOwnProperty('panel') && data.hasOwnProperty('valid') && data.panel === 'dns' && data.valid === true) {
        if (debug) {
            console.log('sending');
        }
        wardPinOut.write(0);
        resetSended = true;
        shouldCheck = false;
        setTimeout(function () {
            if (debug) {
                console.log('low');
            }
            wardPinOut.write(1);
            client.service('states').patch(2, {
                valid: false
            });
            shouldCheck = true;
            resetSended = false;
        }, 1500);
    }
});