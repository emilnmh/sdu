const http = require('http');

const port = process.env.PORT || 3000;

const handler = require('./requestHandler');

const server = http.createServer(async (req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'application/json');
  let reqData = "";
  if(req.method == "POST"){
    const buffers = [];

    for await (const chunk of req) {
      buffers.push(chunk);
    }

    reqData = JSON.parse(Buffer.concat(buffers).toString());
  }

  if(req.method == "OPTIONS"){ //CORS ERROR FIX
    res.setHeader('Access-Control-Allow-Origin', '*')
    res.setHeader('Access-Control-Allow-Methods', '*')
    res.setHeader('Access-Control-Allow-Headers', '*')
    res.end();
  }else{
    res.setHeader('Access-Control-Allow-Origin', '*')
    res.setHeader('Access-Control-Allow-Methods', '*')
    res.setHeader('Access-Control-Allow-Headers', '*')
  }

  if(req.url == "/footballfinder"){
    try {
      retval = await handler.handleRequest(reqData);
      retval = {data: retval};
      res.end(JSON.stringify(retval));
    } catch (error) {
      res.statusCode = error.statusCode;
      res.end(JSON.stringify({error: error.msg}));
    }
  }
  res.statusCode = 400;
  res.end();
  
});

server.listen(port, () => {
  console.log(`Server running at ${port}`);
});