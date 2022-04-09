const express = require('express');
const config = require('config');
const app = express();

const PORT = config.get('port') || 3031;

app.use(express.json({ extended: true }));

app.use('/api/auth', require('./server/auth_routes.js'))

app.listen(PORT, () => {
    console.log(`App has been started on port ${PORT}`);
});