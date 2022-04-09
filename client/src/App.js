import React from 'react'
import {BrowserRouter as Router} from 'react-router-dom'
import {useRoutes} from './pages/routes.js'
import {useAuth} from './hooks/auth.hooks.js'
import {AuthContext} from './context/AuthContext.js'
//import 'materialize-css'

function App() {
  const {token, login, logout, username} = useAuth();
  const isAuthenticated = !!token;
  const routes = useRoutes(isAuthenticated);
  return (
    <AuthContext.Provider value = {{token, login, logout, username, isAuthenticated}}>
    <Router>
      <div className="container">
        {routes}
      </div>
    </Router>
    </AuthContext.Provider>
  );
}

export default App;
