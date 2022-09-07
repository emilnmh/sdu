/**
 * * Functions to handle groups
 */

import { gun, users } from "../index.js";
import { exists, getUUID, readEntry, TIMEOUT } from "./gun_lib.js";

/**
 * @returns true if a given group exists in gunJS
 */
export async function groupExists(group) {
	const timout = new Promise((res) =>
		setTimeout(() => {
			res(false);
		}, TIMEOUT)
	);
	const p = new Promise((resolve) => {
		gun.get("groups").get(group, (ack) => {
			if (!ack.put) resolve(false);
			else resolve(true);
		});
	});
	return await Promise.race([timout, p]);
}

/**
 * @returns list of group member object in a given group.
 */
export async function getGroupMembers(group) {
	const g = gun.get("groups").get(group).get("users");
	var res = [];
	await g
		.once()
		.map()
		.once(
			(i, j) => {
				if (i != undefined && i.name != undefined) {
					res.push({ id: j, name: i.name });
				}
			},
			{ wait: true }
		);
	return res;
}

/**
 * @returns true if group has no members, in other words, if the users field points to nothing.
 */
export async function groupEmpty(group) {
	const r = await exists(["groups", group, "users"], "id");
	return !r;
}

/**
 *	Deletes a group by first going through the elements in the users object in gunJS
 *  and make it point to null. Then doing the same with messages and lastly setting the 'groups'
 * 	node to point to null in that entry, making the group inaccessible and therefore 'deleted'.
 */
export async function deleteGroup(group) {
	const owner = await readEntry(["groups", group], "owner");
	if (users.is && users.is.alias == owner) {
		const g = gun.get("groups").get(group);
		const user_list = await getGroupMembers(group);
		user_list.forEach((element) => {
			g.get("users").get(element.id).put(null);
		});
		const message_max_uid = await getUUID(["groups", group]);
		for (var i = 0; i < message_max_uid; i++) {
			gun.get("groups").get(group).get("messages").get(i).put(null);
		}
		gun.get("groups").get(group).get("messages").put({});
		gun.get("groups").get(group).get("users").put({});
		gun.get("groups").get(group).put(null);
	}
}
