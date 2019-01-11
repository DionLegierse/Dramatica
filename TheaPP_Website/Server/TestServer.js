console.log("AAAA EEFREEF");
const express = require('express')
const cors = require('cors')
const app = express()
const port = 3000

app.use(cors());

app.get('/', (req, res) => {

    if( req.query.message == 'list' )
    {
        var response = {'nodes':[ {'id': '01', 'group': 'una'},
                                  {'id': '02', 'group': 'una'},
                                  {'id': '03', 'group': 'una'}]}
    }
    res.setHeader('Content-Type', 'application/json');
    res.send(JSON.stringify(response));
    console.log(JSON.stringify(response));
/*
    var response = {'ip' : req.ip, 'message' : req.query.message, 'AAAA': req.query.int}
    res.setHeader('Content-Type', 'application/json');
    res.send(JSON.stringify(response));
    console.log(JSON.stringify(response));
*/
});

app.listen(port, () => console.log('Example app listening on port ${port}!'));
