// Establish a Socket.io connection
const feathers = require('@feathersjs/feathers');
const socketio = require('@feathersjs/socketio-client');
const io = require('socket.io-client');

const socket = io('http://192.168.0.11:3030');
const client = feathers();

const Gpio = require('onoff').Gpio;
const button = new Gpio(4, 'in', 'both');

// Initialize our Feathers client application through Socket.io
// with hooks and authentication.

client.configure(socketio(socket));

// send
/*
applicationCache.service('states').patch(2, {
    data: {

    }
});
*/

// listen
console.log('Ok');
const relayMap = {
    'browser': new Gpio(5, 'out'),
    'dns': new Gpio(6, 'out'),
    'bgp': new Gpio(13, 'out'),
    'server': new Gpio(19, 'out'),
};
client.service('states').on('patched', data => {
    if (data.hasOwnProperty('panel') && data.hasOwnProperty('valid')) {
        const panel = data.panel;
        const pin = relayMap[panel];
        pin.writeSync(data.valid ? 1 : 0);
        console.log(pin);
        console.log(data.valid);
    }
});
