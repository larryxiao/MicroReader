var xhrRequest = function (url, type, callback) {
    var xhr = new XMLHttpRequest();
    xhr.onload = function () {
        callback(this.responseText);
    };
    xhr.open(type, url);
    xhr.send();
};

function likeEntry(){
    // Construct URL
    var url = "http://172.27.221.134:3000/like_pebble.json"

        // Send request to OpenWeatherMap
        xhrRequest(url, 'GET', 
                function(responseText) {
                    // responseText contains a JSON object with weather info
                    var json = JSON.parse(responseText);

                    // content 
                    var content = json.summary;      
                    console.log("Receive content: " + content);

                    // Assemble dictionary using our keys
                    var dictionary = {
                        "KEY_CONTENT": content
                    };

                    // Send to Pebble
                    Pebble.sendAppMessage(dictionary,
                        function(e) {
                            console.log("likeEntry successfully!");
                        },
                        function(e) {
                            console.log("likeEntry Pebble!");
                        }
                        );
                }      
    );
}

function dislikeEntry(){
    // Construct URL
    var url = "http://172.27.221.134:3000/dislike_pebble.json"

        // Send request to OpenWeatherMap
        xhrRequest(url, 'GET', 
                function(responseText) {
                    // responseText contains a JSON object with weather info
                    var json = JSON.parse(responseText);

                    // content 
                    var content = json.summary;      
                    console.log("Receive content: " + content);

                    // Assemble dictionary using our keys
                    var dictionary = {
                        "KEY_CONTENT": content
                    };

                    // Send to Pebble
                    Pebble.sendAppMessage(dictionary,
                        function(e) {
                            console.log("dislikeEntry successfully!");
                        },
                        function(e) {
                            console.log("dislikeEntry Pebble!");
                        }
                        );
                }      
    );
}

function getEntry() {
    // Construct URL
    var url = "http://172.27.221.134:3000/pebble_get_feed.json"

        // Send request to OpenWeatherMap
        xhrRequest(url, 'GET', 
                function(responseText) {
                    // responseText contains a JSON object with weather info
                    var json = JSON.parse(responseText);

                    // content 
                    var content = json.summary;      
                    console.log("Receive content: " + content);

                    // Assemble dictionary using our keys
                    var dictionary = {
                        "KEY_CONTENT": content
                    };

                    // Send to Pebble
                    Pebble.sendAppMessage(dictionary,
                        function(e) {
                            console.log("Weather info sent to Pebble successfully!");
                        },
                        function(e) {
                            console.log("Error sending weather info to Pebble!");
                        }
                        );
                }      
    );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
        function(e) {
            console.log("PebbleKit JS ready!");
            getEntry();
        }
        );

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
        function(e) {
            if (e.payload.VOTE_UP) {likeEntry();}
            if (e.payload.VOTE_DOWN) {dislikeEntry();}
            if (e.payload.VOTE_SKIP) {getEntry();}
            console.log("AppMessage received!");
        }                     
        );
