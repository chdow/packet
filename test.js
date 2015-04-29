#!/usr/local/bin/node

var packet = require('./build/Release/packet');

var statusPacket = packet.status();
console.log('status', statusPacket);

var unlockPacket = packet.unlock();
console.log('unlock', unlockPacket);

var lockPacket = packet.lock();
console.log('lock', lockPacket);

var getTimePacket = packet.getTime();
console.log('getTime', getTimePacket);

var epochTimeInS = Date.now() / 1000;
var setTimePacket = packet.setTime(epochTimeInS);
var bufferTime = setTimePacket.readUInt32LE(4);
if (bufferTime != Math.floor(epochTimeInS)) {throw new Error ('bufferTime (' + bufferTime + ') != epochTimeInS (' + epochTimeInS + ')');}
console.log('setTime', setTimePacket);