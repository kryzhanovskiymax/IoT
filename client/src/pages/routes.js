import React from 'react';
import {Switch, Route, Redirect} from 'react-router-dom';
import {Info} from './Info.js'
import {Auth} from './AuthPage.js';


export const useRoutes = isAuthenticated => {
    if (isAuthenticated) {
        return (
            <Switch>
                <Route path="/links" exact>
                    <Info />
                </Route>
                <Route path="/">
                    <Auth />
                </Route>
                <Redirect to="/" />
            </Switch>
        )
    }

    return (
        <Switch>
            <Route path="/" exact>
                <Auth />
            </Route>
            <Redirect to="/"></Redirect>
        </Switch>
    )
}