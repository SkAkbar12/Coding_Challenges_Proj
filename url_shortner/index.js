const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
require('dotenv').config();
require('./model/db.js'); // MongoDB connection

const urlRouter = require('./route/url.route.js');

const app = express();

app.use(cors());
app.use(bodyParser.json());

app.use('/api/url', urlRouter);

const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`✅ Server running on http://localhost:${port}`);
});
