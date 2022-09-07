module.exports.getFields = `SELECT id, description, "location", lat, lon, "type", "owner", event_today FROM fields_vw;`

module.exports.getEvents = `SELECT * FROM events_vw;`

module.exports.getEventById = `SELECT * FROM events_vw WHERE id = $1;`

module.exports.getEventsToday = `SELECT * FROM events_vw WHERE DATE(start_time) = DATE(NOW())` 

module.exports.getEventsOnFieldToday = `SELECT * FROM events_vw WHERE field = $1 AND DATE(start_time) = DATE(NOW())` 

module.exports.getJoinedEventsOnUser = `SELECT * FROM events_vw e
                                        LEFT JOIN participants p ON p.event_id = e.id 
                                        WHERE p.user_id = $1 AND DATE(end_time) = DATE(NOW()) 
                                        ORDER BY e.start_time ASC` 

module.exports.newEvent = `SELECT new_event($1, $2, $3, $4, $5, $6) AS id;` 

module.exports.joinEvent = `SELECT join_event($1, $2) AS joined;` 

module.exports.newUser = `INSERT INTO users (username, password) VALUES($1, $2) RETURNING id, username, password;`

module.exports.userLogin = `SELECT id, username, password 
                                FROM users WHERE username = $1;`