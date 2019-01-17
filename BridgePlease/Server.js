const SerialPort = require('serialport');
const parsers = SerialPort.parsers;
const express = require('express');
const cors = require('cors');
const app = express();
const httpport = 3000;

var isResponseLocked = false;
var response;

//Serial
const parser = new parsers.Readline({
    delimiter: '\r\n',
});

const port = new SerialPort('/dev/serial0', {
    baudRate: 9600,
});

port.pipe(parser);

port.on('open', () => {
    console.log('Port open');
    port.write("{ \"ADD\": 2391144049, \"CMD\": \"T\", \"ARG\": 0 }");
});

parser.on('data', (data) => {
    response = ListNodes(data);
});

//Webserver

app.use(cors());

app.get('/', (req, res) => {
    var message = req.query.message;
    var messageObj = JSON.parse(message);
    console.log(req.query.message);
    console.log(messageObj);
    port.write(message);

    /*while(!(parser.on('data', (data) => {
        response = ListNodes(data);
      }))
    );*/
    if (messageObj.CMD == 'GET') {
        setTimeout(function () {
            res.send(response);
        }, 2000);
    } else {
        res.send(response);
    }

});

app.listen(httpport, () => console.log('listen on ${port}'));


//port.write("{ \"ADD\": 2391144049, \"CMD\": \"T\", \"ARG\": 0 }");


function ListNodes(input) {

    //Rinke code
    response = "";
    var inbuffer = Buffer.from(input);
    var inbufStr = inbuffer.toString();
    var inbufStrArr = inbufStr.split('\r\n');
    var outbuff;
    inbufStrArr.forEach(element => {
        console.log(element);
        outbuff = element;
    });
    isResponseLocked = false;
    return outbuff;

    //JSON.parse(inbuf);
}