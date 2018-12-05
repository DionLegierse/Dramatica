function apply() {
  alert('changes are applied');
}

function discard(){
  alert('changes are discarded');
}

function goBackHome(){
  location.href = "../index.html";
}

function generateTableBridge(a){
  var table = document.getElementById('bridge-info-container');
  var tableData = '<table>';

  for (var i = 0; i < a; i++) {
    tableData += '<tr>';
    tableData += '<td>Option ' + i + '</td>';
    tableData += '<td> <input type="text"> </td>';
    tableData += '</tr>';
  }

  tableData += '</table>';

  table.innerHTML += tableData;
}
