var parseStream = require('./parseStream');

var json = {
    "model": 1,
    "firmware": 1,
    "modes": [
        [
            {
                "altitude": 4500,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 4000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 3000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 2000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 900,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 600,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 300,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            }
        ],
        [
            {
                "altitude": 5000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 4000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 3000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 2000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 900,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 600,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 300,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            }
        ],
        [
            {
                "altitude": 5500,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 4000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 3000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 2000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 900,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 600,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 300,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            }
        ],
        [
            {
                "altitude": 6000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 4000,
                "type": "freefall",
                "sound": 1
            },
            {
                "altitude": 3000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 2000,
                "type": "canopy",
                "sound": 2
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 900,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 600,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 300,
                "type": "freefall",
                "sound": 5
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            },
            {
                "altitude": 0,
                "type": "canopy",
                "sound": 0
            }
        ]
    ]
};

var assert = require("assert");

assert.deepEqual(parseStream.streamToJson(parseStream.jsonToStream(json)).modes,
                 json.modes);