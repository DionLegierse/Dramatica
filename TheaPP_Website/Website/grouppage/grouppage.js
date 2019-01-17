var group = '0';

group = getURLParameter('group');

function goBack()
{
    location.href = "../nodeslist/nodeslist.html"
}

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

function listNodes()
{
    fetchList();

    var nodeList = document.createElement('div');
    for( var i = 0; i < nodes.length; i++ )
    {
        var node = nodes[i];
        if(node.Group == group)
        {
            var but = document.createElement('button');
            but.appendChild(document.createTextNode(node.ADDress));
            but.setAttribute("class", "button-one");
            but.setAttribute("onclick", "goToNode('" + node.Address + "')")
            nodeList.appendChild(but);
        }
    }
    document.getElementById("nodes").appendChild(nodeList)
}

function groupOn()
{
    for( var i = 0; i < nodes.length; i++ )
    {
        var node = nodes[i];
        if( node.Group == group )
        {
            sendHTTPRequest('{"ADD":' + node.ADDress + ',"CMD":"ON","ARG":0}');
        }
    }
}

function groupOff()
{
    for( var i = 0; i < nodes.length; i++ )
    {
        var node = nodes[i];
        if( node.Group == group )
        {
            sendHTTPRequest('{"ADD":' + node.ADDress + ',"CMD":"OFF","ARG":0}');
        }
    }
}

function goToNode( node )
{
    location.href = "../nodepage/nodepage.html?node=" + node + "&group=" + group;
}
