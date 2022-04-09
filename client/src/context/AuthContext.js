import {createContext} from 'react' 


export const AuthContext = createContext({
    token: null,
    username: null,
    login: null,
    logout: null,
    isAuthenticated: false
});