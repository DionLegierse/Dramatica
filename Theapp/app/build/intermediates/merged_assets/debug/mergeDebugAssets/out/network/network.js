function goBackHome(){
    location.href = "../index.html";
}

function sendHTTPRequest() {
    console.log("Attempting to send request");
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", "http://127.0.0.1:3000?message=Hello_World!", true );
    xmlHttp.send( null );
    console.log(xmlHttp.responseText);
}
