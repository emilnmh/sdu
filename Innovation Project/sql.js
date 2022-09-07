const { Client } = require('pg')

module.exports.query = async function(query, param = null) {
    try {
        const client = new Client({
            user: 'jgjbpaon',
            host: 'abul.db.elephantsql.com',
            database: 'jgjbpaon',
            password: 'Q6aaQ2H4LOx2DStdTxa5DkPoVKvpoBXs',
            port: 5432,
          })
        client.connect()
        const res = await client.query(query, param);
        client.end()
        return res;
    } catch (error) {
        console.log(error)
        throw {statusCode: 500, msg: "Internal server error", error: error};
    }
    
}
