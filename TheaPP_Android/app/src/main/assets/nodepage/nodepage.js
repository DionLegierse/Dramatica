var node;
var group;

node  = getURLParameter("node");
group = getURLParameter("group");

function getURLParameter(name)
{
    var pageUrl = window.location.search.substring(1);
    var URLVariables = pageUrl.split('&');
    for( var i = 0; i < URLVariables.length; i++ )
    {
        var paramaterName = URLVariables[i].split('=');
        if( paramaterName[0] == name )
        {
            return paramaterName[1];
        }
    }
}

function toggleNode()
{
    sendHTTPRequest('{"ADD":' + node + ',"CMD":"T","ARG":0}');
    // sendHTTPRequest('{"ADD":2391144049,"CMD":"T","ARG":0}');
}

function setGroup()
{
    var mess = '{"ADD":' + node + ',"CMD":"G","ARG":' + document.getElementById("groupID").value + '}';
    if(document.getElementById("groupID").value)
        sendHTTPRequest(mess);
}

function goBack()
{
    location.href = "../grouppage/grouppage.html?group=" + group;
}
