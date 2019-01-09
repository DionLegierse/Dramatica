function goBackHome(){
    location.href = "../index.html";
}

function sendHTTPRequest() {
    console.log("Attempting to send request");
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "http://192.168.11.14:3000?message=ProjectDomotica", true );
    xmlHttp.send( null );
    console.log(xmlHttp.responseText);
}
