console.log("AAAA EEFREEF");
const express = require('express')
const app = express()
const port = 3000

app.get('/', (req, res) => {
    res.send('<script>alert("Hello! I am an alert box!!");</script>');

});

app.listen(port, () => console.log(`Example app listening on port ${port}!`));