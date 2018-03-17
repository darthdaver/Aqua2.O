var express = require('express');
var app = express();

app.post('/sensor_data', function (req, res) {
  console.log(req.body.data);
  gauge.value = req.body.data;
  res.end();
})

var server = app.listen(8081, function () {

  var host = server.address().address
  var port = server.address().port

  console.log("Example app listening at http://%s:%s", host, port)

})
