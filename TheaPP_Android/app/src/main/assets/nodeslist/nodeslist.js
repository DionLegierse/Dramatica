function goBackHome()
{
    location.href = "../index.html";
}

function goToGroup(group)
{
    location.href = "../grouppage/grouppage.html?group=" + group;
}

function listGroups()
{
  fetchList();

  var groups = [];

  var groupList = document.createElement('div');

  for( var i = 0; i < nodes.length; i++ )
  {
      var node = nodes[i];
      if( node.Group != "0" )
      {
          var foundSame = 0;

          for( var j = 0; j < groups.length; j++ )
          {
              if( node.Group == groups[j] )
              {
                  foundSame = 1;
                  break;
              }
          }

          if( foundSame != 1 )
          {
              addButton(node.Group, groupList);
              groups.push(node.Group);
          }
      }
  }

  addButton("0", groupList);

  document.getElementById("groups").appendChild(groupList)
}

function addButton(text, parent)
{
    var but = document.createElement('button');

    if( text != "0" )
        but.appendChild(document.createTextNode(text));
    else
        but.appendChild(document.createTextNode("Unassigned"));

    but.setAttribute("class", "button-one");
    but.setAttribute("onclick", "goToGroup('" + text + "')")
    parent.appendChild(but);
}
