console.log("AAAA EEFREEF");
const express = require('express')
const app = express()
const port = 3000

app.get('/', (req, res) => {

    var response = {'ip' : req.ip, 'message' : req.query.message, 'AAAA': req.query.int}
    res.setHeader('Content-Type', 'application/json');
    res.send(JSON.stringify(response));
    console.log(JSON.stringify(response));
});

app.listen(port, () => console.log('Example app listening on port ${port}!'));