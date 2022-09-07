import GUN from "gun";
import SEA from "gun/sea.js";
import express from "express";
import bodyParser from "body-parser";
import { loadComponents, request, writeFile } from "./lib/js_lib.js";
import { getProfile, logout, middleware, signin, signup } from "./lib/auth.js";
import { deleteGroup, getGroupMembers, groupExists } from "./lib/groups.js";
import { getUUID } from "./lib/gun_lib.js";
import { legalUserCheck } from "./lib/messages.js";
const app = express();
app.use(express.static("public"));
app.use(express.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(GUN.serve);

const host = "http://localhost";
const port = process.env.PORT ?? 3000;

const entry_node_url = host + ":4999";

const health_check_freq = 5000;

var peers = [];
var relay_peer = "";

/**
 * When the express server has intialized,
 * it gets a relay node to connect to and sync up with its data
 */
var server = app.listen(port, async () => {
	peers.push(host + ":" + port);
	//Join network
	const r = await request(entry_node_url, "/join", {});
	if (JSON.parse(r).relay) {
		peers.push(JSON.parse(r).relay);
		relay_peer = JSON.parse(r).relay;
		const up = await request(JSON.parse(r).relay, "/join", {
			new: host + ":" + port,
		});
		const other_peers = JSON.parse(up).peers;
		other_peers.forEach((n) => peers.push(n));
		console.log("Joined relay node: " + relay_peer);

		gun.opt({
			file: "data" + port,
			multicast: false,
			peers: peers.map((i) => i.concat("/gun")),
			web: server,
			axe: false,
		});

		//Get gunjs data from relay
		if (peers.length > 0) {
			const rel = peers.filter((i) => i != host + ":" + port)[0];
			const fileContents = await request(rel, "/gunjs/file", {}, "GET");
			if (fileContents && fileContents != "false")
				await writeFile(fileContents, port);
		}
		relay_healthCheck();
	}
});

var gun = GUN({
	file: "data" + port,
	multicast: false,
	web: server,
	axe: false,
});
var users = gun.user().recall({ sessionStorage: true });

/**
 * * ---- Enpoints:
 */

/**
 * HTML signup page.
 */
app.get("/", async (req, res) => {
	middleware(
		() => {
			return res.redirect("/chat");
		},
		async () => {
			var html = await loadComponents(["signup"]);
			res.writeHead(200, { "Content-Type": "text/html" });
			res.write(html);
			return res.end();
		}
	);
});

/**
 * HTML login page.
 */
app.get("/login", async (req, res) => {
	middleware(
		() => {
			return res.redirect("/chat");
		},
		async () => {
			const html = await loadComponents(["login"]);
			res.writeHead(200, { "Content-Type": "text/html" });
			res.write(html);
			return res.end();
		}
	);
});

/**
 * Sign up endpoint: (POST, /auth/signup)
 * Creates a new user with the given username and password, and redirected to /login.
 *
 * - username: username of the user.
 * - password: password of the user
 */
app.post("/auth/signup", async (req, res) => {
	const username = req.body.username;
	const password = req.body.password;

	signup(username, password, (bool, error) => {
		if (bool == true) {
			//Send out create user to relay:
			request(relay_peer, "/gunjs/adduser", {
				username: username,
				password: password,
				id: error,
				sender: host + ":" + port,
			});

			res.redirect("/login");
			return res.end();
		} else {
			console.log(error);
			res.redirect("/");
			return res.end();
		}
	});
});

/**
 * Log in endpoint: (POST, /auth/login)
 * User is logged in with the credentials, and redirected to /chat.
 *
 * - username: username of the user.
 * - password: password of the user
 */
app.post("/auth/login", async (req, res) => {
	const username = req.body.username;
	const password = req.body.password;

	signin(username, password, (bool, error) => {
		res.redirect("/chat");
	});
});

/**
 * Log out endpoint: (GET, /auth/logout)
 * User is logged out and redirected to /login.
 */
app.get("/auth/logout", async (req, res) => {
	logout(users);
	res.redirect("/login");
});

/**
 * HTML chat page.
 */
app.get("/chat", async (req, res) => {
	middleware(
		async () => {
			var html = await loadComponents(["main"]);
			res.writeHead(200, { "Content-Type": "text/html" });
			res.write(html);
			return res.end();
		},
		async () => {
			res.redirect("/login");
		}
	);
});

/**
 * HTML chat page.
 */
app.get("/chat/:name", async (req, res) => {
	middleware(
		async () => {
			var html = await loadComponents(["main"]);
			res.writeHead(200, { "Content-Type": "text/html" });
			res.write(html);
			return res.end();
		},
		async () => {
			res.redirect("/login");
		}
	);
});

/**
 * Leave group endpoint: (POST, /leave)
 * User leaves the group.
 */
app.post("/group/leave/:name", async (req, res) => {
	middleware(
		async () => {
			const param = req.params.name;
			const group = gun.get("groups").get(param).get("users");

			const profile_data = await getProfile();
			group.get(profile_data.id).put(null);

			// res.write(JSON.stringify({ message: "success" }));
			res.redirect("/chat");
			return res.end();
		},
		() => {
			res.redirect("/");
		}
	);
});

/**
 * Group endpoint: (GET, /group/:name)
 * Retreives the group with the given name.
 *
 * - :name: The name of the group.
 */
app.get("/group/:name", async (req, res) => {
	const group_name = req.params.name;
	middleware(
		async () => {
			const group_exists = await groupExists(group_name);
			if (group_exists) {
				const group_members = await getGroupMembers(group_name);
				const u = await getProfile();
				const legal_user = legalUserCheck(u, group_members);
				if (legal_user) {
					res.write(JSON.stringify({ members: group_members }));
					return res.end();
				} else {
					return res.redirect("/groups");
				}
			} else {
				return res.redirect("/groups");
			}
		},
		() => {
			return res.redirect("/login");
		}
	);
});

/**
 * Group stream endpoint: (GET, /groups)
 * Creates a stream which sends data when a new group is created.
 */
app.get("/groups", async (req, res) => {
	middleware(
		async () => {
			const headers = {
				"Content-Type": "text/event-stream",
				Connection: "keep-alive",
				"Cache-Control": "no-cache",
			};
			res.writeHead(200, headers);
			var dupes = [];
			gun.get("groups")
				.map()
				.on((i, j) => {
					if (i && !dupes.includes(i)) {
						// gun sometimes fires two event on change, but fixed here
						res.write(
							`event: message\ndata: ${JSON.stringify({
								id: i.id,
								name: i.name,
							})}\n\n`
						);
					}
					dupes.push(i);
				});
		},
		() => {
			res.redirect("/login");
		}
	);
});

/**
 * Create group endpoint: (POST, /group)
 * Creates a new group.
 *
 * - name: Name of the group.
 */
app.post("/group", async (req, res) => {
	const param = req.body.name.toUpperCase();
	if (param.length == 0) return res.redirect("/groups");
	middleware(
		async () => {
			const group = gun.get("groups").get(param);

			const profile_data = await getProfile();
			const profile_object = users.get("profile");

			const e = await groupExists(param);
			if (!e) {
				//group doesnt exist
				group.put({
					id: 0,
					max_uuid: -1,
					owner: profile_data.name,
					name: param,
					users: {},
					messages: {},
				});
				//Used to fix weird bug with GUN
				const uid = await getUUID(["groups", param], "max_uuid");
				group
					.get("groups")
					.get(param)
					.get("messages")
					.get(uid)
					.put("blah");
			}
			group.get("users").get(profile_data.id).put(profile_object);
			res.write(JSON.stringify({ message: "Success" }));
			return res.end();
		},
		() => {
			res.redirect("/login");
		}
	);
});

/**
 * Delete group endpoint. (POST, /delete).
 * Deletes the group.
 */
app.post("/delete", async (req, res) => {
	const name = req.body.name;
	deleteGroup(name);
	res.redirect("/groups");
});

/**
 * Create message endpoint: (POST, /message/:name)
 * Sends a message to a group chat.
 *
 * - :name: Name of the group chat.
 * - msg: Message to be sent.
 */
app.post("/message/:name", async (req, res) => {
	const group_name = req.params.name;
	const msg = req.body.msg;
	const group_exists = await groupExists(group_name);
	if (users.is && group_exists && msg.trim() != "") {
		// checks if valid group and user logged in
		const u = await getProfile();
		const members = await getGroupMembers(group_name);
		const legal_user = legalUserCheck(u, members);
		if (legal_user) {
			const uid = await getUUID(["groups", group_name], "max_uuid");
			// check if user is in group
			const group = gun.get("groups").get(group_name).get("messages");
			const m = {
				author: users.is.alias,
				text: msg,
			};
			group.get(uid).put(m);
			res.write(JSON.stringify({ message: "Success" }));
			return res.end();
		}
	}
});

/**
 * Message stream endpoint: (GET, /groups)
 * Creates a stream which sends data when a message sent to the group.
 */
app.get("/messages/:group", async (req, res) => {
	const group_name = req.params.group;
	const group_members = await getGroupMembers(group_name);
	const u = await getProfile();
	const legal_user = legalUserCheck(u, group_members);
	if (legal_user) {
		const headers = {
			"Content-Type": "text/event-stream",
			Connection: "keep-alive",
			"Cache-Control": "no-cache",
		};
		res.writeHead(200, headers);

		var dupes = [];
		gun.get("groups")
			.get(group_name)
			.get("messages")
			.map()
			.on((i, j) => {
				if (i && !dupes.includes(i)) {
					// gun sometimes fires two event on change, but fixed here
					res.write(
						"event: message\n" +
							"data: " +
							JSON.stringify({
								text: i.text,
								author: i.author,
								time: Math.floor(i["_"][">"]["text"]),
							}) +
							"\n\n"
					);
				}
				dupes.push(i);
			});
	}
});

/**
 * 	Endpoint to add a peer to the list of peers and update the gunJS instance.
 */
app.post("/addpeer", async (req, res) => {
	if (req.body.new) {
		peers.push(req.body.new);
		console.log("Peer added: " + req.body.new);
		gun.opt({
			file: "data" + port,
			multicast: false,
			peers: peers.map((i) => i.concat("/gun")),
			web: server,
			axe: false,
		});
	}
	return res.end();
});

/**
 * 	Endpoint to remove a peer from the list of peers and update the gunJS instance.
 */
app.post("/removepeer", async (req, res) => {
	if (req.body.peer) {
		peers = peers.filter((i) => i != req.body.peer);
		console.log("Peer removed: " + req.body.peer);
		gun.opt({
			file: "data" + port,
			multicast: false,
			peers: peers.map((i) => i.concat("/gun")),
			web: server,
			axe: false,
		});
	}
	return res.end();
});

/**
 *	Endpoint to create a user on the peer.
 *	This is used when another peer is creating a user,
 *	to sync up the user data with the whole network.
 */
app.post("/gunjs/adduser", (req, res) => {
	const username = req.body.username;
	const password = req.body.password;
	const id = req.body.id;

	if (username && password) users.create(username, password, (ack) => {});

	return res.end();
});

/**
 *	Endpoint used by healthchecks from the relay node.
 */
app.get("/status", (req, res) => {
	res.send("ok");
	return res.end();
});

/**
 *	Does a health check with the connected peer at a set interval.
 */
async function relay_healthCheck() {
	await request(relay_peer, "/join", {
		new: host + ":" + port,
	});
	setTimeout(relay_healthCheck, health_check_freq);
}

/**
 *	Listeners for the data in gunJS. This is done to make sure that data persists
 */
gun.get("global")
	.map()
	.on((i) => {});

gun.get("groups")
	.map()
	.on((i) => {});

export { gun, users, port, host };
