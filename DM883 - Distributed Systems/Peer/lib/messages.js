/**
 * * Functions to handle messages
 */

import { TIMEOUT } from "./gun_lib.js";
import { gun } from "../index.js";

/**
 * @returns a list of all messages in a group.
 */
export async function getMessages(group) {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res(false);
		}, TIMEOUT)
	);
	const p = new Promise(async (resolve) => {
		const g = gun.get("groups").get(group).get("messages");
		var res = [];
		await g.map().once(
			(i, j) => {
				if (i != null) {
					res.push({
						id: j,
						author: i.author,
						text: i.text,
						time: i["_"][">"]["text"], //used for causual ordering
					});
				}
			},
			{ wait: true }
		);
		resolve(res);
	});
	return await Promise.race([timout, p]);
}

/**
 * @returns true if there is any messages in a group.
 */
export async function messagesExists(group) {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res(false);
		}, TIMEOUT)
	);
	const p = new Promise(async (resolve) => {
		const msgs = await getMessages(group);
		resolve(msgs.length != 0);
	});
	return await Promise.race([timout, p]);
}

/**
 * Checks if a user exists in a list of users,
 * @returns true if user is in the list of users.
 */
export function legalUserCheck(obj, list) {
	return list.some(
		(person) => person.name == obj.name && person.id == obj.id
	);
}
