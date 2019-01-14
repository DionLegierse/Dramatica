console.log("AAAA EEFREEF");
const express = require('express')
const cors = require('cors')
const app = express()
const port = 3000

app.use(cors());

app.get('/', (req, res) => {

    if( req.query.message == '{"ADD":0,"CMD":"GET","ARG":0}' )
    {
        var response = [{'ID': '01', 'ADDress': '01', 'Group': '0'},
                         {'ID': '02', 'ADDress': '02', 'Group': '1'},
                         {'ID': '03', 'ADDress': '03', 'Group': '2'},
                         {'ID': '04', 'ADDress': '04', 'Group': '2'},
                         {'ID': '05', 'ADDress': '05', 'Group': '1'},
                         {'ID': '06', 'ADDress': '06', 'Group': '1'},
                         {'ID': '07', 'ADDress': '07', 'Group': '3'},
                         {'ID': '08', 'ADDress': '08', 'Group': '2'},
                         {'ID': '09', 'ADDress': '09', 'Group': '0'},
                         {'ID': '0A', 'ADDress': '0A', 'Group': '3'},
                         {'ID': '0B', 'ADDress': '0B', 'Group': '1'},
                         {'ID': '0C', 'ADDress': '0C', 'Group': '3'}]
    } else
    {
        var response = {'ip' : req.ip, 'message' : req.query.message, 'AAAA': req.query.int}
    }

    res.setHeader('Content-Type', 'application/json');
    res.send(JSON.stringify(response));
    console.log(JSON.stringify(response));
});

app.listen(port, () => console.log('Example app listening on port ${port}!'));
