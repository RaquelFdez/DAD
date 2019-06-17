function getData(selector){
    var estadoTabla = document.getElementById('estadoTabla');
    var request = new XMLHttpRequest;
    request.open('GET', '/comidas');
    request.onload = function () {
        var data = JSON.parse(this.response);

        if(request.status >= 200 && request.status < 400){

                var columns = addAllColumnHeaders(data, selector);

                for (var i = 0; i < data.length; i++) {
                  var row$ = $('<tr/>');
                  for (var colIndex = 0; colIndex < columns.length; colIndex++) {
                    var cellValue = data[i][columns[colIndex]];
                    if (cellValue == null) cellValue = "";
                    row$.append($('<td/>').html(cellValue));
                  }
                  $(selector).append(row$);
                }
             estadoTabla.innerHTML += 'Datos cargados correctamente';

        }else{
           estadoTabla.innerHTML += 'No se pudieron cargar los datos';
        }
    };
    request.onerror = function(){
        estadoTabla.innerHTML += 'Fallo de conexión';
    }
    request.send();
}

function addAllColumnHeaders(myList, selector) {
    var columnSet = [];
    var headerTr$ = $('<tr/>');

    for (var i = 0; i < myList.length; i++) {
      var rowHash = myList[i];
      for (var key in rowHash) {
        if ($.inArray(key, columnSet) == -1) {
          columnSet.push(key);
          headerTr$.append($('<th/>').html(key));
        }
      }
    }
    $(selector).append(headerTr$);

    return columnSet;
  }

  function eliminar(){
  	var id = document.getElementById('idComida').value;
  	var json = JSON.stringify({idComida : new Number(id)});
  	var xhr = new XMLHttpRequest();
    alert("ID Comida:"+id);
  	xhr.open('DELETE', '/borrar');
  	xhr.send(json);
  	xhr.onerror = function(){alert('fallo');}

  }
function sendForm(){
  
	$.post("/add", JSON.stringify({
		nombre: new String(document.getElementById('nombre').value),
		horas: new Number((document.getElementById('hora').value) + document.getElementById('minutos').value),
		pesosPlato : new Number(document.getElementById('pesoPlato').value)}),
		
	function(){
		alert("Datos insertados");
	},"json").fail(function(){alert("Error")});

	$(document).ready(function(){
		$(document.body).load()
	})
  }
