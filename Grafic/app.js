// JavaScript source code
const express = require("express");

let app = express();

app.use(express.static("./")).listen(3000);