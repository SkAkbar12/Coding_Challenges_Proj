const express = require('express');
const { createUrl, getAll, redirectTo } = require('../controller/url.controller');

const router = express.Router();

router.route('/')
  .post(createUrl)
  .get(getAll);

router.route('/:sendid')
  .get(redirectTo);

module.exports = router;
