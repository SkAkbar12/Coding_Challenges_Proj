const mongoose = require('mongoose');
require('dotenv').config();

const url = process.env.MONGODB_URL;

mongoose.connect(url)
  .then(() => console.log("✅ Connected to MongoDB"))
  .catch((err) => console.log("❌ Error connecting to MongoDB:", err));

module.exports = mongoose;
