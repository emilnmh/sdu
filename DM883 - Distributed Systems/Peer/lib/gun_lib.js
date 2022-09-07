/**
 * * General gunDB functions
 */

import { gun } from "../index.js";

export const TIMEOUT = 300;

/**
 * Accesses the object through the chain list parameter and
 * checks if the last object in the chain has a certain object
 * @returns true if the object was found in gunJS.
 */
export async function exists(chain, object) {
	if (chain.length == 0) return false;
	var rsc = gun.get(chain[0]);
	for (var i = 1; i < chain.length; i++) {
		rsc = rsc.get(chain[i]);
	}
	var res = true;
	await rsc
		.once()
		.get(object)
		.once(
			(i) => {
				if (!i) res = false;
			},
			{ wait: true }
		);
	return res;
}

/**
 * Finds the field with max_uuid in gunJS and increments it if it exists,
 * else creates it.
 * @returns a uuid.
 */
export async function getUUID(chain, name = "max_uuid") {
	if (chain.length == 0) return false;
	var rsc = gun.get(chain[0]);
	for (var i = 1; i < chain.length; i++) {
		rsc = rsc.get(chain[i]);
	}
	var res = await exists(chain, name);
	//max_UUID is not found, create it
	if (!res) {
		rsc.put({ max_uuid: 0 });
	}
	//Get max_uuid
	var res_max_uuid = 0;
	rsc.get(name).once(
		(i) => {
			res_max_uuid = i;
		},
		{ wait: true }
	);
	//update max_uuid and return
	rsc.put({ max_uuid: res_max_uuid + 1 });
	return res_max_uuid;
}

/**
 * @returns list of objects at the end of the chain in gunJS.
 */
export async function read(chain) {
	if (chain.length == 0) return [];
	var rsc = gun.get(chain[0]);
	for (var i = 1; i < chain.length; i++) rsc = rsc.get(chain[i]);

	var result = [];
	await rsc
		.once()
		.map()
		.once((i) => result.push(i), { wait: true });
	return result;
}

/**
 * Goes through the chain in gunJS and looks for a certain entry and returns that if it exists.
 * @returns an object.
 */
export async function readEntry(chain, entry) {
	if (chain.length == 0) return [];
	var rsc = gun.get(chain[0]);
	for (var i = 1; i < chain.length; i++) rsc = rsc.get(chain[i]);

	const timout = new Promise((res) =>
		setTimeout(() => {
			res(null);
		}, TIMEOUT)
	);
	const p = new Promise(async (resolve) => {
		rsc.get(entry, (ack) => {
			if (ack.put) resolve(ack.put);
			else resolve(null);
		});
	});
	return await Promise.race([timout, p]);
}
