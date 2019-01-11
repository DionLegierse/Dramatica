function apply() {
  var snackbar = document.getElementById("snackbar");

  snackbar.innerHTML = "Changes have been applied";

  snackbar.className = "animate";
  setTimeout(function(){
    snackbar.className = snackbar.className.replace("animate", "");
    snackbar.innerHTML = "";
  }, 3000);
}

function discard(){
  var snackbar = document.getElementById("snackbar");

  snackbar.innerHTML = "Changes have been discarded";

  snackbar.className = "animate";
  setTimeout(function(){
    snackbar.className = snackbar.className.replace("animate", "");
    snackbar.innerHTML = "";
  }, 3000);
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
