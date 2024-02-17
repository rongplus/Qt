var splitter,cont1,cont2;
var last_x,window_width;
var map ;

//band list
var band_list = [];
var band_marker = new Map();

function create_html_items()
{	
 
   band_list.forEach(function(element) {

    	var item = document.createElement("TD");
    	item.innerHTML = element;
    	document.getElementById("field_name").appendChild(item);


    	var item1 = document.createElement("TD");
    	var checkbox = document.createElement('input');
		checkbox.type = "checkbox";
		checkbox.name = "name";
		checkbox.value = element;
		checkbox.id = "id";
		var group = band_marker.get(element);
		

		var label = document.createElement('label');
		label.htmlFor = "id";
		label.appendChild(document.createTextNode( band_marker.get(element).length ));

		item1.appendChild(checkbox);
		item1.appendChild(label);

    var clr = document.createElement('input');
    clr.type = "color";
    clr.value = "#off0000";
    item1.appendChild(clr);

    checkbox.onclick = function() {
      handleClick(this, band_marker.get(element),clr.value);
        // access properties using this keyword   
          console.log("no 0---------");    
    };

		document.getElementById("field_group").appendChild(item1);
 
	});
}
	
function getCircle(magnitude,color)
{
    return {
      path: google.maps.SymbolPath.CIRCLE,
      fillColor: color,
      fillOpacity: .6,
      scale: Math.pow(2, magnitude) / 2,
      strokeColor: 'white',
      strokeWeight: .5
    };
}
	  
function init()
{
    window_width=window.innerWidth;
    splitter=document.getElementById("splitter");
    cont1=document.getElementById("div2");
    cont2=document.getElementById("div1");
    var dx=cont1.clientWidth;
    splitter.style.marginLeft=dx+"px";
    dx+=splitter.clientWidth;
    cont2.style.marginLeft=dx+"px";
    dx=window_width-dx;
    cont2.style.width=dx+"px";
    splitter.addEventListener("mousedown",spMouseDown);
    google.charts.load("current", {packages:["corechart"]});
    

}

function spMouseDown(e)
{
    splitter.removeEventListener("mousedown",spMouseDown);
    window.addEventListener("mousemove",spMouseMove);
    window.addEventListener("mouseup",spMouseUp);
    last_x=e.clientX;
}

function spMouseUp(e)
{
    window.removeEventListener("mousemove",spMouseMove);
    window.removeEventListener("mouseup",spMouseUp);
    splitter.addEventListener("mousedown",spMouseDown);
    resetPosition(last_x);
}

function spMouseMove(e)
{
    resetPosition(e.clientX);
}

function resetPosition(nowX)
{
    var dx=nowX-last_x;
    dx+=cont1.clientWidth;
    cont1.style.width=dx+"px";
    splitter.style.marginLeft=dx+"px";
    dx+=splitter.clientWidth;
    cont2.style.marginLeft=dx+"px";
    dx=window_width-dx;
    cont2.style.width=dx+"px";
    last_x=nowX;
    google.maps.event.trigger(map, "resize");
}

function initMap()
{
    map = new google.maps.Map(document.getElementById('div2'), {
    zoom: 8,
    center: {lat:43.643794, lng:-79.613486}
    });
}

//create a marker
function createMarker(group,name,lng,lat,clr,tip,e)
{
    var latLng = new google.maps.LatLng(  lat,lng);
    var image = 'https://developers.google.com/maps/documentation/javascript/examples/full/images/beachflag.png';
    var marker = new google.maps.Marker({
            position: latLng,
            map: null,
            label: name,
            icon: getCircle(5,clr)
          });

    var infowindow = new google.maps.InfoWindow({
      content: tip
    });

    marker.addListener('click', function() {
      if(url != "")
        window.location.href = "http://www.baidu.com";
      else if( tip != "")
      {
        map.setZoom(8);
        map.setCenter(marker.getPosition());
        infowindow.open(marker.get('map'), marker);
      }
     
    });

    group.push(marker);
}
// Removes/shows the markers from the map, but keeps them in the array.
function showMarkers(group,m,color)
{
    for (var i = 0; i < group.length; i++)
    {
        group[i].setMap(m);
        group[i].setIcon( getCircle(3.5,color) )
    }
}
 
 //checkbox click event
function handleClick(cb,group,color)
{
    var p = null;
    console.log("clicked");
    if(cb.checked)
    {
        p = map;
    }
 
    showMarkers(group,p,color);
}

function Creat_Maker(group, name, clr,lng,lat,tip, e)
{
   if(band_list.indexOf(group) <= -1)
  {
    band_list.push(group);
    var m = [];
    band_marker.set( group, m);
  }

  createMarker(band_marker.get(group),name,lng,lat,clr,tip,e);

  console.log("index: " + band_list.indexOf(group));
}

function read_sheet(wb,sheetName)
{
   var name;
    var group;

    var tput;
    var lng;
    var lat;
    var clr;
    var tip;
    var url;

    var rowObj =XLSX.utils.sheet_to_row_object_array(wb.Sheets[sheetName]);
    var jsonObj = JSON.stringify(rowObj);

    console.log( document.getElementById("bandname").value);
    // ----------------- read every cell ----------------------------------------------
    JSON.parse(jsonObj, (key, value) =>
    {
          //console.log(key); // log the current property name, the last is "".

          if( key.trim().toUpperCase() == "NAME")
            name = value.trim();
          else if(key.trim().toUpperCase()  == "GROUP")
            group = value.trim();
          else if( key.trim().toUpperCase()  == "LONG")
            lng = value.trim();
          else if(key.trim().toUpperCase()  == "LAT" )
            lat = value;  
          else if (key.trim().toUpperCase()  == "TIP" )
            tip = value;
          else if(key.trim().toUpperCase() == "COLOR")
            clr = value;
          else if( key.trim().toUpperCase() == "EVENT")
            url = value;
          else if( key.trim().toUpperCase()  == "END")
          {
              Creat_Maker(group, name, 'red',lng,lat,"tip", "e");            
              group = "";     
          }
          //console.log(value);
          //console.log('---------------\n');
          return value;     // return the unchanged property value.
    });
    //console.log(jsonObj)
    //console.log('---------------\n');

   create_html_items();
}

//------------ read xml file ------------------------------
function ExcelExport(event)
{
   

    var index = 0;
    var input = event.target;
    var reader = new FileReader();
    reader.onload = function()
    {
        var fileData = reader.result;
        var wb = XLSX.read(fileData, {type : 'binary'});
        // --------------- read every sheet ----------------------------------------------
        wb.SheetNames.forEach(function(sheetName)
        {
            read_sheet(wb,sheetName);
        });

    };

    var res = reader.readAsBinaryString(input.files[0]);

    console.log('---------------\n');
}



