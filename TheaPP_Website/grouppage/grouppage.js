var json = {'a': 'Cas', 'b': 'Is', 'c': 'Gay'};

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

function goBack()
{
    location.href = "../nodeslist/nodeslist.html"
}
