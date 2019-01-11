var group = 'x';

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
        if(node.group == group)
        {
            var but = document.createElement('button');
            but.appendChild(document.createTextNode(node.id));
            but.setAttribute("class", "button-one");
            but.setAttribute("onclick", "goToNode('" + node.id + "')")
            nodeList.appendChild(but);
        }
    }
    document.getElementById("nodes").appendChild(nodeList)
}

function goToNode( node )
{
    location.href = "../nodepage/nodepage.html?node=" + node + "&group=" + group;
}
