var nodes;

function fetchList()
{
    sendHTTPRequest("list");
}


function sendHTTPRequest(message)
{
    console.log("Attempting to send request");

    var url = "http://127.0.0.1:3000?message=" + message;

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
