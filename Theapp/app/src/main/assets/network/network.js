function goBackHome(){
    location.href = "../index.html";
}

function sendHTTPRequest() {
    console.log("Attempting to send request");
    var xmlHttp = new XMLHttpRequest();
    /*
    xmlHttp.onreadystatechange = function() {
            if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
                callback(xmlHttp.responseText);
        }
        */
    xmlHttp.open( "GET", "http://pi:3000/?message=AllesInTypenWatJeWilt&int=69", true );
    xmlHttp.send();
    console.log(xmlHttp.responseText);
}