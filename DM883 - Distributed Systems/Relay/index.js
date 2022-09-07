import GUN from "gun";
import SEA from "gun/sea.js";
import http from "http";
import express from "express";
import fs from "fs";
const app = express();
app.use(express.json());
app.use(GUN.serve);
const host = "http://localhost";
const port = process.env.PORT ?? 4000;

const entry_node_url = host + ":4999";

const health_check_freq = 5000;

var user_peers = [];
var relay_peers = [];

/**
 * 	The express server initializes and tells the entry node that it exists.
 * 	It then gets all other relay nodes from the entry node and adds it to its peer list.
 *	To sync up with the other relays, it gets the storage contents and saves writes it to its own storage.
 */
const server = app.listen(port, async () => {
	if (fs.existsSync("./data" + port + "/!"))
		fs.unlinkSync("./data" + port + "/!");
	//Tell entry node that I exist:
	const r = await request(entry_node_url, "/addrelay", {
		url: host,
		port: port,
	});
	relay_peers = JSON.parse(r).relay_list;

	gun.opt({
		file: "data" + port,
		multicast: false,
		peers: relay_peers
			.map((i) => i.concat("/gun"))
			.concat(user_peers.map((i) => i.concat("/gun"))),
		web: server,
		axe: false,
	});

	//sync up with other relays:
	if (relay_peers.length > 1) {
		const rel = relay_peers.filter((i) => i != host + ":" + port)[0];
		const fileContents = await request(rel, "/gunjs/file", {}, "GET");
		if (fileContents && fileContents != "false")
			await writeFile(fileContents);
	} else {
		gun.get("global").put({ max_user_id: 0 });
	}
	entry_healthCheck();
	users_healthCheck();
});

var gun = GUN({
	file: "data" + port,
	multicast: false,
	web: server,
	axe: false,
});
var users = gun.user().recall({ sessionStorage: true });

/**
 *	Endpoint for when another relay is joining the network.
 * 	Updates the peer list and gunJS instance.
 */
app.post("/addrelay", (req, res) => {
	if (req.body.new) {
		if (!relay_peers.includes(req.body.new)) {
			relay_peers.push(req.body.new);
			gun.opt({
				file: "data" + port,
				multicast: false,
				peers: relay_peers
					.map((i) => i.concat("/gun"))
					.concat(user_peers.map((i) => i.concat("/gun"))),
				web: server,
				axe: false,
			});
		}
	}
	res.send("ok");
	return res.end();
});

/**
 * 	Endpoint for when a new peer is connecting to the relay. Adds the peer
 * 	to the list of peer nodes and updates the gun instance.
 * 	Also tells all other connected peers to connect to the new peer.
 */
app.post("/join", async (req, res) => {
	if (req.body.new) {
		if (!user_peers.includes(req.body.new)) {
			user_peers.push(req.body.new);
			console.log(
				"Added peer " + req.body.new + " to the list of peers."
			);
			gun.opt({
				file: "data" + port,
				multicast: false,
				peers: relay_peers
					.map((i) => i.concat("/gun"))
					.concat(user_peers.map((i) => i.concat("/gun"))),
				web: server,
				axe: false,
			});

			const other_users = user_peers.filter((i) => i != req.body.new);
			other_users.forEach(async (u) => {
				await request(u, "/addpeer", { new: req.body.new });
			});

			res.send({ peers: other_users });
			return res.end();
		}
	} else {
		res.send({ peers: [] });
		return res.end();
	}
});

/**
 *	Endpoint
 */
app.get("/gunjs/file", async (req, res) => {
	const fileContents = await loadFile();
	res.send(fileContents);
	return res.end();
});

/**
 * 	Endpoint for when a peer creates a user. It propagates out the request to all other relays
 * 	and peers connected.
 */
app.post("/gunjs/adduser", (req, res) => {
	const username = req.body.username;
	const password = req.body.password;
	const id = req.body.id;
	const sender = req.body.sender;
	if (username && password && id) {
		const p_list = user_peers.filter((i) => i != sender);
		const r_list = relay_peers.filter((i) => i != host + ":" + port);

		p_list.forEach(async (n) => {
			await request(n, "/gunjs/adduser", {
				username: username,
				password: password,
				id: id,
			});
		});

		if (sender) {
			r_list.forEach(async (n) => {
				await request(n, "/gunjs/adduser", {
					username: username,
					password: password,
					id: id,
				});
			});
		}
	}
	res.send("ok");
	return res.end();
});

/**
 *	Endpoint used for healthchecks by peers and the entry node.
 */
app.get("/status", (req, res) => {
	res.send("ok");
	return res.end();
});

/**
 * Sends a http request to a given url and path, with some data.
 * @returns the response to the request.
 */
async function request(url, path, data, method = "POST") {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res(false);
		}, 1000)
	);
	const p = new Promise((resolve) => {
		const peer_port = url.substring(url.lastIndexOf(":") + 1);
		// const peer_host = url.substring(0, url.lastIndexOf(":"));
		const options = {
			host: "localhost", //should be peer_host if not on localhost
			port: peer_port,
			path: path,
			headers: {
				"content-type": "application/json",
			},
			method: method,
		};

		const request = http.request(options, (response) => {
			response.setEncoding("utf8");
			response.on("data", (data) => {
				if (response.statusCode === 200) {
					resolve(data);
				}
			});
		});
		request.on("error", (err) => {
			console.log(err);
		});
		// if (method.toUpperCase() === "POST")
		if (!(Object.keys(data).length === 0))
			request.write(JSON.stringify(data));
		request.end();
	});
	return await Promise.race([timout, p]);
}

/**
 * 	Reads the content of the gunJS storage file
 * @returns returns the data stored in the ! file
 */
async function loadFile() {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res(false);
		}, 500)
	);
	const p = new Promise((resolve) => {
		fs.readFile("./data" + port + "/!", function (err, data) {
			if (!err) {
				resolve(data.toString());
			} else {
				resolve(false);
			}
		});
	});
	return await Promise.race([timout, p]);
}

/**
 * Writes a string to a ! file which is the storage of gunJS. This is used
 * to sync up when a newly created relay or peer is joining.
 * @returns true if file was successfully written to.
 */
async function writeFile(data) {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res("Could not write data to " + port);
		}, 500)
	);
	const p = new Promise((resolve) => {
		fs.writeFile(
			"./data" + port + "/!",
			data,
			{
				encoding: "utf8",
				flag: "w",
			},
			(err) => {
				if (err) resolve(err);
				resolve(true);
			}
		);
	});
	return await Promise.race([timout, p]);
}

/**
 * 	Health check for the entry node. In case of the entry crashing, and restarting
 * 	the relay keeps telling the entry that it is up, so the entry node can add
 * 	it to its list of relay nodes.
 */
async function entry_healthCheck() {
	await request(entry_node_url, "/addrelay", {
		url: host,
		port: port,
	});
	setTimeout(entry_healthCheck, health_check_freq);
}

/**
 *	Health check for connected peers. If a peer is down, send a signal
 *  to all other peers connected to the relay to remove the down peer from their lists of peers.
 * 	Also updates the gun instance of the relay node.
 */
async function users_healthCheck() {
	if (user_peers.length > 0) {
		user_peers.forEach(async (n) => {
			const res = await request(n, "/status", {}, "GET");
			if (res != "ok") {
				const the_rest = user_peers.filter((i) => i != n);
				user_peers = user_peers.filter((i) => i != n);
				console.log("Removed peer " + n + " from the list of peers.");
				gun.opt({
					file: "data" + port,
					multicast: false,
					peers: relay_peers
						.map((i) => i.concat("/gun"))
						.concat(user_peers.map((i) => i.concat("/gun"))),
					web: server,
					axe: false,
				});
				the_rest.forEach((p) => {
					request(p, "/removepeer", { peer: n });
				});
			}
		});
	}
	setTimeout(users_healthCheck, health_check_freq);
}

/**
 *	Listeners for the data in gunJS. This is done to make sure that all the data persists
 */
gun.get("global")
	.map()
	.on((i) => {});

gun.get("groups")
	.map()
	.on((i) => {
		if (i) {
			gun.get("groups")
				.get(i.name)
				.get("messages")
				.map()
				.on((i) => {});

			gun.get("groups")
				.get(i.name)
				.get("users")
				.map()
				.on((i) => {});
		}
	});
