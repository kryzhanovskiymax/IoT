const { Router } = require('express');
const { userExists, createUser, checkUser } = require('./routes/db.js');
const jwt = require('jsonwebtoken');
const config = require('config');

const router = Router();

router.post('/register', async (req, res) => {
    try {
        console.log('Body: ', req.body);
        //console.log(req.body.isEmpty());

        const { username, password } = req.body;

        //if (username)
        const exists = await userExists(username);

        if (!exists) {
            console.log(`user ${username} not exists`);
            let response = await createUser(username, password);
            return res.status(201).json({message: "Completed"});
        } else {
            console.log(`user ${username} exists`);
            return res.status(400).json({message: "User already exists"});
        }

    } catch(e) {
        return res.status(500).json({message: 'Something went Wrong'});
    }
});

router.post('/login', async (req, res) => {
    try {

        const { username, password } = req.body;


        const exists = await checkUser(username, password);

        if (exists) {
            const token = jwt.sign(
                { userId: username }, config.get('jwtSecret'), {expiresIn: '1h'}
            );

            return res.status(201).json({token, name: username});
        } else {
            return res.status(400).json({message: "Wrong email or password"});
        }

    } catch(e) {
        return res.status(500).json({message: 'Something went Wrong'});
    }
});

module.exports = router;