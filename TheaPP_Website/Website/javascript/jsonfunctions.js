var nodes;

function fetchList()
{
    sendHTTPRequest('{"ADD":0,"CMD":"GET","ARG":0}');
}


function sendHTTPRequest(message)
{
    console.log("Attempting to send request");

    var url = "http://192.168.11.14:3000?message=" + message;

    console.log(url);

    var xhr = createCORSRequest('GET', url);

    if(!xhr) {
        alert('CORS not supported');
        return;
    }

    /*
     *  HTTP request handlers
     */
    xhr.onload = function() {
        var text = xhr.responseText;

        if( message == '{"ADD":0,"CMD":"GET","ARG":0}' )
            nodes = JSON.parse(text);
    };

    xhr.onerror = function() {
        alert('An error occured during the making of the request.');
    };

    xhr.send();
}

function createCORSRequest(method, url)
{
    var xhr = new XMLHttpRequest();
    if("withCredentials" in xhr) {
        // XHR for Chrome/Firefox/Opera/Safari.
        xhr.open(method, url, false);
    } else if (typeof XDomainRequest != "undefined") {
        // XDomainRequest for Internet Explorer
        xhr = new XDomainRequest();
        xhr.open(method, url);
    } else {
        // CORS is not supported
        xhr = null;
    }

    return xhr;
}
