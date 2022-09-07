# Peer Node

This is a peer node for group A11 in DM883

## Installation

```bash
npm i
```

## Run a peer node

When the desired relay and entry nodes are up and running, run the following command to start a peer node:

```bash
PORT=[port] node index.js
```

If `PORT` is not specified, the default port will be `3000`.

It is not necessary to have relay or entry nodes to run a peer, but it cannot connect to other peers without it.

Access the front-end by going into a browser (preferably Google Chrome) and go to the URL:
`http://localhost:[port]`

## Create a user

GunJS requires that the password of the user is not "too easy", so if you are redirected back to the sign up page after signing up, try a longer password.
