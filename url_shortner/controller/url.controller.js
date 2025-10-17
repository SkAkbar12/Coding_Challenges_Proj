const { nanoid } = require('nanoid');
const Url = require('../model/url.model');

const createUrl = async (req, res) => {
  try {
    const { url } = req.body;

    if (!url) {
      return res.status(400).json({
        success: false,
        message: "Please send a valid URL",
      });
    }

    let shortId = nanoid(6);
    let shorturl=`http://localhost:8888/api/url/${shortId}`
    const newUrl = new Url({ shortId, redirect: url,shorturl });

    await newUrl.save();

    return res.status(200).json({
      success: true,
      message: "URL created successfully",
      data: newUrl,
    });
  } catch (err) {
    return res.status(500).json({
      success: false,
      message: err.message,
    });
  }
};

const getAll = async (req, res) => {
  try {
    const all = await Url.find();
    return res.status(200).json({
      success: true,
      message: "All URLs fetched successfully",
      data: all,
    });
  } catch (err) {
    return res.status(500).json({
      success: false,
      message: err.message,
    });
  }
};

const redirectTo = async (req, res) => {
  try {
    const { sendid } = req.params;
    const found = await Url.findOne({ shortId: sendid });

    if (!found) {
      return res.status(404).json({
        success: false,
        message: "URL not found",
      });
    }

    found.visited += 1;
    await found.save();

    // Redirect to the original URL
    return res.redirect(found.redirect);

  } catch (err) {
    return res.status(500).json({
      success: false,
      message: err.message,
    });
  }
};

module.exports = {
  createUrl,
  getAll,
  redirectTo
};
