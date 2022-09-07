/**
 * * General JS functions
 */

import fs from "fs";
import http from "http";

/**
 * Loads a html file, used to send the front-end code to the browser.
 */
export async function loadHTML(component) {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res("Could not load " + component);
		}, 500)
	);
	const p = new Promise((resolve) => {
		fs.readFile(
			"./components/" + component + ".html",
			function (err, data) {
				if (!err) {
					resolve(data);
				}
			}
		);
	});
	return await Promise.race([timout, p]);
}

/**
 * Loads a list of html components and concatonates it.
 * @returns html string
 */
export async function loadComponents(components) {
	var html = await loadHTML(components[0]);
	for (var i = 1; i < components.length; i++)
		html += await loadHTML(components[i]);
	return html;
}

/**
 * Sends a http request to a given url and path, with some data.
 * @returns the response to the request.
 */
export async function request(url, path, data, method = "POST") {
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
		if (!(Object.keys(data).length === 0))
			request.write(JSON.stringify(data));
		request.end();
	});
	return await Promise.race([timout, p]);
}

/**
 * Writes a string to a ! file which is the storage of gunJS. This is used
 * to sync up when a newly created relay or peer is joining.
 * @returns true if file was successfully written to.
 */
export async function writeFile(data, port) {
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
