const sqlCon = require('./sql');

const bcrypt = require('bcryptjs');
const rounds = 10;

const sqlStatements = require('./sql-statements');

module.exports.handleRequest = async function (data){
    switch(data.case){
        case "getfields":
            return await getFields();
        case "getevents":
            return await getEvents();
        case "geteventstoday":
            return await getEventsToday();
        case "geteventsonfieldtoday":
            return await getEventsOnFieldToday(data.field_id);
        case "getjoinedevents":
            return await getJoinedEventsOnUser(data.user_id);
        case "geteventbyid":
            return await getEventByID(data.id);
        case "newevent":
            return await newEvent(data.field_id, data.max_particpants, data.owner, data.desc, data.start, data.end);
        case "joinevent":
            return await joinEvent(data.event_id, data.user_id);
        case "newuser":
            return await newUser(data.username, data.password);
        case "userlogin":
            return await userLogin(data.username, data.password);
    }

}

async function getFields(){
    const fields = await sqlCon.query(sqlStatements.getFields);
    return fields.rows;
}

async function getEvents(){
    const events = await sqlCon.query(sqlStatements.getEvents);
    return events.rows;
}

async function getEventsToday(){
    const events = await sqlCon.query(sqlStatements.getEventsToday);
    return events.rows;
}

async function getEventsOnFieldToday(field_id){
    if(!field_id){
        throw {statusCode: 400, msg: "Bad request"};
    }
    const events = await sqlCon.query(sqlStatements.getEventsOnFieldToday, [field_id]);
    return events.rows;
}

async function getJoinedEventsOnUser(user_id){
    if(!user_id){
        throw {statusCode: 400, msg: "Bad request"};
    }
    const events = await sqlCon.query(sqlStatements.getJoinedEventsOnUser, [user_id]);
    return events.rows;
}

async function getEventByID(id){
    const event = await sqlCon.query(sqlStatements.getEventById, [id]);
    return {event: event.rows[0]};
}

async function newEvent(field_id, max_particpants, owner, desc, start, end){
    if(!field_id || !max_particpants || !owner || !desc || !start || !end){
        throw {statusCode: 400, msg: "Bad request"};
    }
    const eventID = await sqlCon.query(sqlStatements.newEvent, [field_id, max_particpants, owner, desc, start, end]);
    let id = eventID.rows[0].id;

    const event = await sqlCon.query(sqlStatements.getEventById, [id]);

    if(event.rows[0] == null){
        throw {statusCode: 409, msg: "Event overlaps another event"};
    }else{
        return {event: event.rows[0]};
    }
}

async function joinEvent(event_id, user_id){
    if(!event_id || !user_id){
        throw {statusCode: 400, msg: "Bad request"};
    }
    const joined = await sqlCon.query(sqlStatements.joinEvent, [event_id, user_id]);

    if(joined.rows[0].joined){
        return "Event joined";
    }else{
        throw {statusCode: 409, msg: "Already joined or too many participants"};
    }
}

async function newUser(username, password){
    if(!username || !password){
        throw {statusCode: 400, msg: "Bad request"};
    }
    const hash = bcrypt.hashSync(password, rounds);
    try {
        const user = await sqlCon.query(sqlStatements.newUser, [username, hash]);
        return {user: user.rows[0]};
    } catch (error) {
        if(error.error.routine === "_bt_check_unique"){
            throw {statusCode: 409, msg: "Username is already taken"};
        }else{
            throw error;
        }
    }
    
}

async function userLogin(username, password){
    if(!username || !password){
        throw {statusCode: 401, msg: "Invalid login"};
    }
    const res = await sqlCon.query(sqlStatements.userLogin, [username]);
    const user = res.rows[0];

    if(user != null && bcrypt.compareSync(password, user.password)){
        return {user: user};
    }else{
        throw {statusCode: 401, msg: "Invalid login"};
    }

}

