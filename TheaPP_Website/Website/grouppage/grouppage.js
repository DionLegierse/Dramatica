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
        if(node.group == "una")
        {
            var but = document.createElement('button');
            but.appendChild(document.createTextNode(node.id));
            but.setAttribute("class", "button-one");
            nodeList.appendChild(but);
        }
    }
    document.getElementById("nodes").appendChild(nodeList)
}

function debugNodes()
{
    console.log(nodes);
}

/*
document.getElementById("groups").appendChild(fetchList(json));

function fetchList()
{
    var buttonList = document.createElement('div');
    for( var i = 0; i < Object.keys(json).length; i++ )
    {
        var but = document.createElement('button');
        but.appendChild(document.createTextNode(Object.values(json)[i]));
        but.setAttribute("class", "button-one");
        buttonList.appendChild(but);
    }
    return buttonList;
}
*/
