const mongoose = require('mongoose');

const urlSchema = new mongoose.Schema(
  {
    shortId: {
      type: String,
      required: true,
      unique: true
    },
    redirect: {
      type: String,
      required: true,
    },
    visited: {
      type: Number,
      default: 0,
    },
    shorturl:{
        type:String,
        
    }
  },
  { timestamps: true }
);

module.exports = mongoose.model('URL', urlSchema);

