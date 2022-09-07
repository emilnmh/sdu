import express from "express";
import http from "http";
const app = express();
app.use(express.json());
const port = process.env.PORT ?? 4999;

const health_check_freq = 5000;

var relays = [];

const server = app.listen(port, () => {
	console.log("Entry Node at port: " + port);
});

/**
 *	Endpoint for adding a relay to the list of relays. Also tells all other relays in the
 *	network to sync up with the new relay.
 */
app.post("/addrelay", (req, res) => {
	if (req.body.url && req.body.port) {
		const newRelay = {
			url: req.body.url + ":" + req.body.port,
			priority: 1
		};

		if (!relays.some(e => e.url === newRelay.url)) {
			console.log("Added " + newRelay.url + " as a relay node");
			relays.push(newRelay);
			if (relays.length > 1) {
				//Tell the other relays to connect to the new relay.
				const neighbours = relays.filter((i) => i != newRelay);
				neighbours.forEach(async (r) => {
					await request(r.url, "/addrelay", { new: newRelay.url });
				});
			}
		}
		res.statusCode = 200;
	} else {
		res.statusCode = 400;
	}

	var relay_list = relays.map(worker => worker.url);
	res.send(JSON.stringify({ relay_list: relay_list }));
	return res.end();
});


/**
 * 	Endpoint for the user to request a relay node to join.
 */
app.post("/join", (req, res) => {
	if (relays.length > 0) {
		var totalPriority = 0;
		for (var i = 0; i < relays.length; i++) {
			totalPriority = relays[i].priority;
		}

		var random_value = Math.floor(Math.random() * totalPriority) + 1;
		var index = null;
		for (var i = 0; i < relays.length; i++) {
			random_value -= relays[i].priority;
			if (index === null && random_value <= 0) {
				index = i;
				relays[i].priority = 1;
			} else {
				relays[i].priority += 1;
			}
		}

		const worker = relays[index].url;
		res.send(JSON.stringify({ relay: worker }));
		return res.end();
	} else {
		res.statusCode = 400;
		res.send("No availble workers");
		return res.end();
	}
});

/**
 * Returns a random worker.
 * Each time that a worker is not picked its priority is increased by 1.
 * If it is picked the priority is set back to 1.
 * The higher the priority the greater is the chance of getting picked.
 * @returns
 */
function get_assigned_worker(activeWorkers) {
	var totalPriority = 0;
	for (const worker in activeWorkers) {
		totalPriority += worker.priority;
	}
	var index = null;
	var random_value = Math.floor(Math.random() * totalPriority);
	for (var i = 0; i < activeWorkers.length; i++) {
		random_value = value - workers[index].priority;
		if (index === null && random_value < 0) {
			index = i;
			workers[i].priority = 1;
		} else {
			workers[i].priority++;
		}
	}
	return { url: workers[index].url, port: workers[index].port };
}

/**
 * 	Health check for all relay nodes. If a relay node is not responding it is removed
 * 	from the list of relay nodes.
 */
function healthCheck() {
	if (relays.length > 0) {
		relays.forEach(async (n) => {
			const res = await request(n.url, "/status", {}, "GET");
			if (res != "ok") {
				relays = relays.filter((i) => i != n);
				console.log("Removed " + n.url + " as a relay node");
			}
		});
	}
	setTimeout(healthCheck, health_check_freq);
}
healthCheck();

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
			//console.log(err);
		});
		if (!(Object.keys(data).length === 0))
			request.write(JSON.stringify(data));
		request.end();
	});
	return await Promise.race([timout, p]);
}
