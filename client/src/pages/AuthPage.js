import React, {useEffect, useState, useContext} from 'react';
import {AuthContext} from '../context/AuthContext.js'
import {useHttp} from '../hooks/http.hook'

export const Auth = () => {
    const auth = useContext(AuthContext);
    const { loading, error, request } = useHttp();
    const [form, setForm] =useState({
        username: '', password: ''
    });

    useEffect(() => {

    }, [error]);

    const changeHandler = event => {
        setForm({ ...form, [event.target.name]: event.target.value})
    }

    const registerHandler = async () => {
        try {
            const data = await request('api/auth/register', 'POST', {...form});

        } catch(e) {
            alert(e);
        } 
    }

    const loginHandler = async () => {
        try {
            const data = await request('api/auth/login', 'POST', {...form});
            auth.login(data.token, data.name);
            alert('You have succesfully registered');
        } catch(e) {
            alert(e);
        } 
    }

    return (
        <div className="auth-wrapper">
            <div className="auth">
                <div class="title">
                    <h1>Graph API</h1>
                    <h3>Developed by Maxim Kryzhanovskiy</h3>
                </div>
                <div className="auth-form">
                    <div className="input-fields">
                        <div className="input-form">
                            <label htmlFor="username">Name</label>
                            <input placeholder="username" id="username" className="username-field" name="username" onChange={changeHandler}></input>
                        </div>
                        <div className="input-form">
                            <label htmlFor="password">Password</label>
                            <input placeholder="password" id="username" className="username-field" name="password" onChange={changeHandler}></input>    
                        </div>
                    </div>
                    <div class="buttons">
                        <button className="login" onClick={loginHandler} disabled={loading}>Log in</button>
                        <button className="register" onClick={registerHandler} disable={loading}>Register</button>
                    </div>
                </div>
            </div>
        </div>
    )
} 