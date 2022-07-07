const express = require('express');
const app = express();
const port = 3000;

app.use(express.static(__dirname + '/template'));

app.get('/', (req, res) => {
	res.sendFile(__dirname + '/template/index.html');
});

app.get('/download', (req, res) => {
    const file = __dirname + '/StayHealthy.zip';
    res.download(file);
});

app.listen(port, () => {
	console.log('Listening on port ${port}');
});
