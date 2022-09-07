import { getUUID } from "./gun_lib.js";
import { users } from "../index.js";

/**
 * 	Accesses the currently logged in users profile data.
 * @returns Profile object containing name and id of the user.
 */
export async function getProfile() {
	var result = {};
	await users
		.get("profile")
		.map()
		.once(
			(i, j) => {
				result[j] = i;
			},
			{ wait: true }
		);
	return result;
}

/**
 * 	Creates a user in gunJS and creates the necessary profile data.
 */
export function signup(username, password, _callback) {
	users.create(username, password, (ack) => {
		if (ack.err) {
			//user already exists
			_callback(false, ack.err);
		} else {
			users.auth(username, password, async (ack) => {
				const uid = await getUUID(["global", "max_user_id"]);
				users.get("profile").put({ name: username, id: uid }, (ack) => {
					users.leave();
					_callback(true, uid);
				});
			});
		}
	});
}

/**
 *	Logs in the user, if not successful the callback function is called with an error.
 */
export async function signin(username, password, _callback) {
	users.auth(username, password, (ack) => {
		if (ack.err) _callback(false, ack.err);
		else _callback(true);
	});
}

/**
 * Logs out the user.
 */
export function logout() {
	users.leave();
}

/**
 *	Middleware used in the endpoints. So some code is run when a user is logged in
 *	and some other code is run when a guest uses the endpoints.
 */
export async function middleware(_user, _guest) {
	if (users.is) _user();
	else _guest();
}
