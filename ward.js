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
const wardPinIn = new Gpio(17, 'in', 'rising');
const wardPinOut = new Gpio(27, 'out');

console.log('started');
wardPinIn.watch(function (err, value) { //
    console.log('new');// Watch for hardware interrupts on pushButton GPIO, specify callback function
    if (err) { //if an error
        console.error('There was an error', err); //output error message to console
    }
    console.log(value);

    applicationCache.service('states').patch(2, {
        valid: true
    });
});

/*
applicationCache.service('states').patch(2, {
    data: {

    }
});
*/
